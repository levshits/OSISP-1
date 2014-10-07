#pragma once
#include "Instrument.h"
class Grabe :
	public Instrument
{
public:
	static Grabe* GetInstance()
	{
		static Grabe _self;
		
		return &_self;
	}
	virtual void Draw(int x, int y)
	{
		HPEN newPen = CreatePen(NULL, Instrument::Width, RGB(255, 255, 255));
		HPEN temp = (HPEN)SelectObject(Instrument::DeviceDC, newPen);
		DeleteObject(temp);
		temp = (HPEN)SelectObject(Instrument::MemoryDC, newPen);
		DeleteObject(temp);
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

