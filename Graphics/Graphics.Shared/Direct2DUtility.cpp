#include "pch.h"
#include "Direct2DUtility.h"

using namespace DX;

Direct2DHelper::Direct2DHelper()
{
}

Direct2DHelper::~Direct2DHelper()
{
}

// Creates a Direct2D bitmap from the specified file name.
HRESULT Direct2DHelper::LoadBitmapFromFile(
	ID2D1DeviceContext *renderTarget,
    const wchar_t *uri,
    unsigned int destinationWidth, 
    unsigned int destinationHeight,
    ID2D1Bitmap1 **bitmap)
{
    HRESULT hr = S_OK;

    ComPtr<IWICBitmapDecoder> decoder;
    ComPtr<IWICBitmapFrameDecode> bitmapSource;
    ComPtr<IWICStream> stream;
    ComPtr<IWICFormatConverter> converter;
    ComPtr<IWICBitmapScaler> scaler;
    ComPtr<IWICImagingFactory> wicFactory;

    hr = GetWICFactory(&wicFactory);
    if (SUCCEEDED(hr))
    {
        hr = wicFactory->CreateDecoderFromFilename(
            uri,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &decoder);
    }

    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = decoder->GetFrame(0, &bitmapSource);
    }

    if (SUCCEEDED(hr))
    {
        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = wicFactory->CreateFormatConverter(&converter);
    }

    if (SUCCEEDED(hr))
    {
        // If a new width or height was specified, create an
        // IWICBitmapScaler and use it to resize the image.
        if (destinationWidth != 0 || destinationHeight != 0)
        {
            unsigned int originalWidth, originalHeight;
            hr = bitmapSource->GetSize(&originalWidth, &originalHeight);
            if (SUCCEEDED(hr))
            {
                if (destinationWidth == 0)
                {
                    float scalar = static_cast<float>(destinationHeight) / static_cast<float>(originalHeight);
                    destinationWidth = static_cast<unsigned int>(scalar * static_cast<float>(originalWidth));
                }
                else if (destinationHeight == 0)
                {
                    float scalar = static_cast<float>(destinationWidth) / static_cast<float>(originalWidth);
                    destinationHeight = static_cast<unsigned int>(scalar * static_cast<float>(originalHeight));
                }

                hr = wicFactory->CreateBitmapScaler(&scaler);
                if (SUCCEEDED(hr))
                {
                    hr = scaler->Initialize(
                        bitmapSource.Get(),
                        destinationWidth,
                        destinationHeight,
                        WICBitmapInterpolationModeCubic);
                }
                if (SUCCEEDED(hr))
                {
                    hr = converter->Initialize(
						scaler.Get(),
                        GUID_WICPixelFormat32bppPBGRA,
                        WICBitmapDitherTypeNone,
                        nullptr,
                        0.f,
                        WICBitmapPaletteTypeMedianCut);
                }
            }
        }
        else // Don't scale the image.
        {
            hr = converter->Initialize(
				bitmapSource.Get(),
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                nullptr,
                0.f,
                WICBitmapPaletteTypeMedianCut);
        }
    }

    if (SUCCEEDED(hr))
    {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = renderTarget->CreateBitmapFromWicBitmap(
			converter.Get(),
            nullptr,
            bitmap);
    }

    return hr;
}

//
// Get a WIC Imaging factory
//
HRESULT Direct2DHelper::GetWICFactory(IWICImagingFactory** factory)
{
    static ComPtr<IWICImagingFactory> m_pWICFactory;
    HRESULT hr = S_OK;

    if (nullptr == m_pWICFactory)
    {
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pWICFactory));
    }

    if (SUCCEEDED(hr))
    {
		*factory = m_pWICFactory.Get();
    }

    return hr;
}