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

namespace Hilo
{
    namespace Direct2DHelpers
    {
        //
        // This class provides various utility functions to work with 
        // Direct2D and related APIs such as WIC and DirectWrite
        //
        class Direct2DUtility
        {
        public:
            static HRESULT LoadBitmapFromFile(ID2D1RenderTarget *renderTarget, const wchar_t *uri, unsigned int destinationWidth, unsigned int destinationHeight, ID2D1Bitmap **bitmap);
            static HRESULT GetWICFactory(__out IWICImagingFactory** factory);
			
			template<class T>
			inline static HRESULT AssignToOutputPointer(T** pp, const ComPtr<T> &p)
			{
				assert(pp);
				*pp = p;
				if (nullptr != (*pp))
				{
					(*pp)->AddRef();
				}

				return S_OK;
			}

        private:
            Direct2DUtility();
            ~Direct2DUtility();
        };
    }
}
