#pragma once
#include "Instrument.h"
class Pen :	public Instrument
{
private:
	Pen(){};
public:
	static Pen* GetInstance()
	{
		static Pen _self;
		HPEN newPen = CreatePen(NULL, Instrument::Width, Instrument::PenColor);
		HGDIOBJ temp = SelectObject(Instrument::DeviceDC, newPen);
		DeleteObject(temp);
		temp = SelectObject(Instrument::MemoryDC, newPen);
		DeleteObject(temp);
		return &_self;
	}
	virtual void Draw(int x, int y)
	{
		MoveToEx(MemoryDC, ToMemoryDCX(previous_x), ToMemoryDCY(previous_y), NULL);
		LineTo(MemoryDC, ToMemoryDCX(x), ToMemoryDCY(y));
		previous_x = x;
		previous_y = y;
		ZoomAndMove::DisplayMemoryDC();
	}
	virtual void Display(int x, int y)
	{
		Draw(x, y);
	}
};

