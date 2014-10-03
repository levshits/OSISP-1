#pragma once
#include "Instrument.h"
class Oval :
	public Instrument
{
private:
	Oval(){};
public:
	static Oval* GetInstance()
	{
		static Oval _self;
		return &_self;
	}
	virtual void Draw(int x, int y)
	{
		Ellipse(MemoryDC, x, y, previous_x, previous_y);
		BitBlt(DeviceDC, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, MemoryDC, canvasRect.left, canvasRect.top, SRCCOPY);
	}
	virtual void Display(int x, int y)
	{
		BitBlt(DeviceDC, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, MemoryDC, canvasRect.left, canvasRect.top, SRCCOPY);
		Ellipse(DeviceDC, x, y, previous_x, previous_y);
	}

};

