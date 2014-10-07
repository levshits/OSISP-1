#pragma once
#include "Instrument.h"

	class Line :
		public Instrument
	{
	private:
		Line(){};
	public:
		static Line* GetInstance()
		{
			static Line _self;
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
			ZoomAndMove::DisplayMemoryDC();
			MoveToEx(DeviceDC, previous_x, previous_y, NULL);
			LineTo(DeviceDC, x, y);
		}

	};


