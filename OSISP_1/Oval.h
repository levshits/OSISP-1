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
		Ellipse(MemoryDC, ToMemoryDCX(x), ToMemoryDCY(y), ToMemoryDCX(previous_x), ToMemoryDCY(previous_y));
		ZoomAndMove::DisplayMemoryDC();
	}
	virtual void Display(int x, int y)
	{
		ZoomAndMove::DisplayMemoryDC();
		Ellipse(DeviceDC, x, y, previous_x, previous_y);
	}

};

