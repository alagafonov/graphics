#pragma once

#include "DirectXHelper.h"
#include "ScreenBuffer.h"

/*#define VIRTUAL_WIDTH 320
#define VIRTUAL_HEIGHT 200
#define SCALE 1*/

#define VIRTUAL_WIDTH 1366
#define VIRTUAL_HEIGHT 768
#define SCALE 0

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

// Renders Direct2D and 3D content on the screen.
namespace Graphics
{
	class GraphicsMain
	{
	private:
		// Direct3D objects.
		ComPtr<ID3D11Device1> m_d3dDevice;
		ComPtr<ID3D11DeviceContext1> m_d3dContext;
		ComPtr<IDXGISwapChain1> m_swapChain;
		
		// Direct3D rendering objects. Required for 3D.
		ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;

		// Direct2D drawing components.
		ComPtr<ID2D1Device> m_d2dDevice;
		ComPtr<ID2D1DeviceContext> m_d2dContext;
		ComPtr<ID2D1Factory1> m_d2dFactory;
		ComPtr<ID2D1Bitmap1> m_d2dTargetBitmap;

		// Cached device properties.
		D3D_FEATURE_LEVEL m_d3dFeatureLevel;

		// Contains the debugging level of an ID2D1Factory object.
		D2D1_FACTORY_OPTIONS options;
		DXGI_PRESENT_PARAMETERS dxgiPresentParameters;

		// Cached reference to the Window.
		Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

		// Screen DPI properties.
		float dpiX, dpiY;

		// Screen buffer object.
		ScreenBuffer *screenBuffer;
	
	public:
		void Initialize();
		void Update();
		void Render();
	};
}
