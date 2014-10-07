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
		HPEN newPen = CreatePen(NULL, Instrument::Width, Instrument::PenColor);
		HGDIOBJ temp = SelectObject(Instrument::DeviceDC, newPen);
		DeleteObject(temp);
		temp = SelectObject(Instrument::MemoryDC, newPen);
		DeleteObject(temp);
		return &_self;
	}
	virtual void Draw(int x, int y)
		{
		Rectangle(MemoryDC, ToMemoryDCX(x), ToMemoryDCY(y), ToMemoryDCX(previous_x), ToMemoryDCY(previous_y));
		ZoomAndMove::DisplayMemoryDC();
			//previous_x = x;
			//previous_y = y;
			//InvalidateRect(Canvas, NULL, false);
		}
		virtual void Display(int x, int y)
		{
			ZoomAndMove::DisplayMemoryDC();
			Rectangle(DeviceDC, x, y, previous_x, previous_y);
		}

};

