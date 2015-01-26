#include "pch.h"
#include "ScreenBuffer.h"

using namespace D2D1;

ScreenBuffer::ScreenBuffer(int width, int height, ID2D1DeviceContext *d2d1DeviceContext, float dpiX, float dpiY)
{
	this->width = width;
	this->height = height;

	// Create scene bitmap properties.
	D2D1_BITMAP_PROPERTIES1 sceneBitmapProperties = BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY
		);

	// Create scene bitmap with predefined width and height.
	d2d1DeviceContext->CreateBitmap(SizeU(width, height), nullptr, 0, sceneBitmapProperties, &sceneBitmap);
	
	// Create byte array for our pixel buffer.
	screen = new byte[width * height * PIXEL_SIZE];

	// Create effect buffer.
	fBuffer = new byte[width * height * PIXEL_SIZE];
	memset(fBuffer, 0, width * height * PIXEL_SIZE);

	// Clear newly created screen buffer.
	Clear();
}

ScreenBuffer::~ScreenBuffer()
{
	// Clean up after ourselves.
	delete[] screen;
	screen = NULL;
}

void ScreenBuffer::Clear()
{
	// Clear the pixel buffer.
	memset(screen, 0, width * height * PIXEL_SIZE);
}

void ScreenBuffer::SetPixel(int x, int y, RGBAColour &color)
{
	// Calculate position of pixel in the byte array.
	int index = width * PIXEL_SIZE * y + x * PIXEL_SIZE;

	// Set pixel by setting colour in BGRA order.
	screen[index] = color.b;
	screen[index + 1] = color.g;
	screen[index + 2] = color.r;
	screen[index + 3] = color.a;
}

RGBAColour ScreenBuffer::GetPixel(int x, int y)
{
	// Calculate position of pixel in the byte array.
	int index = width * PIXEL_SIZE * y + x * PIXEL_SIZE;
	return RGBAColour(screen[index + 2], screen[index + 1], screen[index], screen[index + 3]);
}

ID2D1Bitmap1 *ScreenBuffer::GetBitmap()
{
	// Copy byte array to bitmap and return it.
	sceneBitmap->CopyFromMemory(&RectU(0, 0, width, height), screen, width * PIXEL_SIZE);
	return sceneBitmap.Get();
}

void ScreenBuffer::FireEffect()
{
	RGBAColour c1, c2, c3, c4;
	int index;
	for (int y = 1; y < height - 2; y++)
	{
		for (int x = 1; x < width - 2; x++)
		{
			c1 = GetPixel(x + 1, y);
			c2 = GetPixel(x - 1, y);
			c3 = GetPixel(x, y + 1);
			c4 = GetPixel(x, y - 1);

			// Calculate position of pixel in the byte array.
			index = width * PIXEL_SIZE * (y - 1) + x * PIXEL_SIZE;

			// Set pixel by setting colour in BGRA order.
			fBuffer[index] = (c1.b + c2.b + c3.b + c4.b) / 4;
			fBuffer[index + 1] = (c1.g + c2.g + c3.g + c4.g) / 4;
			fBuffer[index + 2] = (c1.r + c2.r + c3.r + c4.r) / 4;
			fBuffer[index + 3] = (c1.a + c2.a + c3.a + c4.a) / 4;
		}
	}

	memcpy(screen, fBuffer, width * height * PIXEL_SIZE);
}