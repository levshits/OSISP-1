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
			MoveToEx(MemoryDC, previous_x, previous_y, NULL);
			LineTo(MemoryDC, x, y);
			previous_x = x;
			previous_y = y;
			BitBlt(DeviceDC, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, MemoryDC, canvasRect.left, canvasRect.top, SRCCOPY);
		}
		virtual void Display(int x, int y)
		{
			BitBlt(DeviceDC, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, MemoryDC, canvasRect.left, canvasRect.top, SRCCOPY);
			MoveToEx(DeviceDC, previous_x, previous_y, NULL);
			LineTo(DeviceDC, x, y);
		}

	};


