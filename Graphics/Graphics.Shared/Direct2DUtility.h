//===================================================================================
// Copyright (c) Microsoft Corporation.  All rights reserved.                        
//                                                                                   
// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY                    
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT                       
// LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND                          
// FITNESS FOR A PARTICULAR PURPOSE.                                                 
//===================================================================================

#pragma once
#include <algorithm>
#include <d2d1.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

namespace DX
{
    //
    // This class provides various utility functions to work with 
    // Direct2D and related APIs such as WIC and DirectWrite
    //
	class Direct2DHelper
    {
    public:
		static HRESULT LoadBitmapFromFile(ID2D1DeviceContext *renderTarget, const wchar_t *uri, unsigned int destinationWidth, unsigned int destinationHeight, ID2D1Bitmap1 **bitmap);
        static HRESULT GetWICFactory(__out IWICImagingFactory** factory);

    private:
		Direct2DHelper();
		~Direct2DHelper();
    };
}
