#pragma once
#include <Windows.h>
#include "ZoomAndMove.h"
class Instrument
{
protected:
	int previous_x, previous_y;
	Instrument(){};
	~Instrument(){};
public:
	static double ZoomCoefficient;
	static int x_startdisplay;
	static int y_startdisplay;
	static DWORD PenColor, BrushColor;
	static LOGFONT *logFont;
	static HDC MemoryDC, DeviceDC;
	static HBITMAP Buffer;
	static RECT canvasRect;
	static HWND Canvas;
	static int Width;
	virtual void Draw(int x, int y) = 0;
	virtual void Display(int x, int y) = 0;
	void Initialize(int x, int y)
	{
		previous_x = x;
		previous_y = y;
		
	}

	static int ToMemoryDCX(int x)
	{
		return x/ZoomCoefficient + x_startdisplay;
	}

	static int ToMemoryDCY(int y)
	{
		return y/ZoomCoefficient + y_startdisplay;
	}
};
