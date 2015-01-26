#pragma once

#define PIXEL_SIZE 4

using namespace Microsoft::WRL;

struct RGBAColour
{
	byte r, g, b, a;
	
	RGBAColour()
	{
		r = g = b = a = NULL;
	}

	RGBAColour(byte iR, byte iG, byte iB, byte iA)
	{
		r = iR;
		g = iG;
		b = iB;
		a = iA;
	}
};

class ScreenBuffer
{
private:
	int width, height;
	byte *screen;
	ComPtr<ID2D1Bitmap1> sceneBitmap;
public:
	ScreenBuffer(int width, int height, ID2D1DeviceContext *d2d1DeviceContext, float dpix, float dpiy);
	~ScreenBuffer();
	void SetPixel(int x, int y, RGBAColour &color);
	RGBAColour GetPixel(int x, int y);
	ID2D1Bitmap1 *GetBitmap();
	void Clear();
	void FireEffect();
};