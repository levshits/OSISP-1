#pragma once
#include "Instrument.h"
class Polygon :
	public Instrument
{
private:
	Polygon(){};
public:
	static Polygon* GetInstance()
	{
		static Polygon _self;
		return &_self;
	}
	virtual void Draw(int x, int y)
		{
		Rectangle(MemoryDC, x, y, previous_x, previous_y);
		BitBlt(DeviceDC, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, MemoryDC, canvasRect.left, canvasRect.top, SRCCOPY);
			//previous_x = x;
			//previous_y = y;
			//InvalidateRect(Canvas, NULL, false);
		}
		virtual void Display(int x, int y)
		{
			BitBlt(DeviceDC, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, MemoryDC, canvasRect.left, canvasRect.top, SRCCOPY);
			Rectangle(DeviceDC, x, y, previous_x, previous_y);
		}

};

