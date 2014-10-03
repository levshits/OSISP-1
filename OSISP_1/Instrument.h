#pragma once
#include <Windows.h>
class Instrument
{
protected:
	int previous_x, previous_y;
	Instrument(){};
	~Instrument(){};
public:
	
	static DWORD PenColor, BrushColor;
	static HDC MemoryDC, DeviceDC;
	static HBITMAP Buffer;
	static RECT canvasRect;
	HWND Canvas;
	int Width;
	virtual void Draw(int x, int y) = 0;
	virtual void Display(int x, int y) = 0;
	void Initialize(int x, int y)
	{
		previous_x = x;
		previous_y = y;
	}
};
