#include "pch.h"
#include "GraphicsMain.h"

using namespace Graphics;
using namespace D2D1;
using namespace DX;

// Loads and initializes DirectX componenets and application assets when the application is loaded.
void GraphicsMain::Initialize()
{
	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// This array defines the set of DirectX hardware feature levels this app  supports.
	// The ordering is important and you should  preserve it.
	// Don't forget to declare your app's minimum required feature level in its
	// description.  All apps are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = D3D11CreateDevice(
		nullptr,					// Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
		0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,				// Set debug and Direct2D compatibility flags.
		featureLevels,				// List of feature levels this app can support.
		ARRAYSIZE(featureLevels),	// Size of the list above.
		D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps.
		&device,					// Returns the Direct3D device created.
		&m_d3dFeatureLevel,			// Returns feature level of device created.
		&context					// Returns the device immediate context.
		);

	if (FAILED(hr))
	{
		return;
	}

	// Store pointers to the Direct3D 11.1 API device and immediate context.
	DX::ThrowIfFailed(
		device.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		context.As(&m_d3dContext)
		);

	// Obtain the underlying DXGI device of the Direct3D11 device.
	ComPtr<IDXGIDevice1> dxgiDevice;
	DX::ThrowIfFailed(
		device.As(&dxgiDevice)
		);

	// Create ID2D1Factory1 factory to create Direct2D resources.
	DX::ThrowIfFailed(
			D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&options,
			&m_d2dFactory
			)
		);

	// Obtain the Direct2D device for 2-D rendering.
	DX::ThrowIfFailed(
		m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
		);

	// Get Direct2D device's corresponding device context object.
	DX::ThrowIfFailed(
		m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext
			)
		);

	// Get current screen DPI.
	m_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	// Allocate a descriptor.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;                                  // use double buffering to enable flip
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // this is the most common swapchain format
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;    // all apps must use this SwapEffect
	swapChainDesc.SampleDesc.Count = 1;                             // don't use multi-sampling
	swapChainDesc.SampleDesc.Quality = 0;

	// Identify the physical adapter (GPU or card) this device is runs on.
	ComPtr<IDXGIAdapter> dxgiAdapter;
	DX::ThrowIfFailed(
		dxgiDevice->GetAdapter(&dxgiAdapter)
		);

	// Get the factory object that created the DXGI device.
	ComPtr<IDXGIFactory2> dxgiFactory;
	DX::ThrowIfFailed(
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
		);

	// Get pointer to window.
	m_window = CoreWindow::GetForCurrentThread();

	// Get the final swap chain for this window from the DXGI factory.
	DX::ThrowIfFailed(
		dxgiFactory->CreateSwapChainForCoreWindow(
			device.Get(),
			reinterpret_cast<IUnknown*>(m_window.Get()),
			&swapChainDesc,
			nullptr,    // allow on all displays
			&m_swapChain
			)
		);

	// Ensure that DXGI doesn't queue more than one frame at a time.
	DX::ThrowIfFailed(
		dxgiDevice->SetMaximumFrameLatency(1)
		);

	// Get the backbuffer for this window which is be the final 3D render target.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

	// Now we set up the Direct2D render target bitmap linked to the swapchain. 
	// Whenever we render to this bitmap, it is directly rendered to the 
	// swap chain associated with the window.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		dpiX,
		dpiY
		);

	// Direct2D needs the dxgi version of the backbuffer surface pointer.
	ComPtr<IDXGISurface> dxgiBackBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
		);

	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmapFromDxgiSurface(
			dxgiBackBuffer.Get(),
			&bitmapProperties,
			&m_d2dTargetBitmap
			)
		);

	// Now we can set the Direct2D render target.
	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
}

// this function performs updates to the state of the game
void GraphicsMain::Update()
{
}

// this function renders a single frame of 3D graphics
void GraphicsMain::Render()
{
	// Start drawing.
	m_d2dContext->BeginDraw();

	// Clear screen.
	m_d2dContext->Clear(ColorF(0.0f, 0.0f, 0.0f));

	// End drawing.
	DX::ThrowIfFailed(
		m_d2dContext->EndDraw()
		);

	// Swap buffers.
	DX::ThrowIfFailed(
		m_swapChain->Present1(1, 0, &dxgiPresentParameters)
		);
}