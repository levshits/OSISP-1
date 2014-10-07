#include "stdafx.h"
#include "ZoomAndMove.h"
#include  "Instrument.h"
#include "UI.h"

void ZoomAndMove::CreateCanvas(HWND hwnd, HINSTANCE hinstance, int width, int height)
{
	realWidth = width;
	realHeight = height;
	RECT rect = { 0, 0, 0, 0 };
	GetClientRect(hwnd, &rect);
	rect.right -= 2 * CANVAS_OFFSET_X;
	rect.bottom -= TOOLBAR_HEIGHT;
	if ((width <= rect.right) && (height <= rect.bottom))
	{
		UI::CreateCanvas(hwnd, hinstance, width, height);
		Instrument::ZoomCoefficient = 1;
	}
	else
	{

		double proportionsDisplay = static_cast<double>(rect.right) / rect.bottom;
		double proportionsImage = static_cast<double>(width) / height;
		double coefficient = 0;
		if (proportionsDisplay > proportionsImage)
		{
			coefficient = (double)rect.bottom / height ;
			
		}
		else
			coefficient = (double)rect.right / width;
		Instrument::ZoomCoefficient = coefficient;
		UI::CreateCanvas(hwnd, hinstance, width * coefficient, height * coefficient);
	}
	UI::CreateCanvasMemoryDC(hwnd, hinstance, width, height);
	DisplayMemoryDC();

}

void ZoomAndMove::UpdateCanvas(HWND hwnd, HINSTANCE hinstance)
{
	int width = realWidth;
	int height = realHeight;
	RECT rect = { 0, 0, 0, 0 };
	GetClientRect(hwnd, &rect);
	rect.right -= 2 * CANVAS_OFFSET_X;
	rect.bottom -= TOOLBAR_HEIGHT;
	if ((width <= rect.right) && (height <= rect.bottom))
	{
		UI::CreateCanvas(hwnd, hinstance, width, height);
		Instrument::ZoomCoefficient = 1;
	}
	else
	{

		double proportionsDisplay = static_cast<double>(rect.right) / rect.bottom;
		double proportionsImage = static_cast<double>(width) / height;
		double coefficient = 0;
		if (proportionsDisplay > proportionsImage)
		{
			coefficient = (double)rect.bottom / height;

		}
		else
			coefficient = (double)rect.right / width;
		Instrument::ZoomCoefficient = coefficient;
		UI::CreateCanvas(hwnd, hinstance, width * coefficient, height * coefficient);
	}
	DisplayMemoryDC();
}

void ZoomAndMove::DisplayMemoryDC()
{
	UpdateWindow(Instrument::Canvas);
	StretchBlt(Instrument::DeviceDC,
		Instrument::canvasRect.left,
		Instrument::canvasRect.top,
		Instrument::canvasRect.right,
		Instrument::canvasRect.bottom,
		Instrument::MemoryDC,
		Instrument::x_startdisplay,
		Instrument::y_startdisplay,
		Instrument::canvasRect.right/Instrument::ZoomCoefficient,
		Instrument::canvasRect.bottom/Instrument::ZoomCoefficient,
		SRCCOPY);
	
}

void ZoomAndMove::Zoom(HWND hwnd, HINSTANCE hinstance, double value)
{
	value = value / 480;
	RECT rect = { 0, 0, 0, 0 };
	GetClientRect(hwnd, &rect);
	rect.right -= 2 * CANVAS_OFFSET_X;
	rect.bottom -= TOOLBAR_HEIGHT;
	value += Instrument::ZoomCoefficient;
	if (value > 0)
	{

		Instrument::ZoomCoefficient = value;
		int height = realHeight * Instrument::ZoomCoefficient;
		int width = realWidth * Instrument::ZoomCoefficient;
		if (rect.right > width)
		{
			rect.right = width;
		}
		if (rect.bottom > height)
		{
			rect.bottom = height;
		}
		UI::CreateCanvas(hwnd, hinstance, rect.right, rect.bottom);
		if (width - Instrument::x_startdisplay*Instrument::ZoomCoefficient < (Instrument::canvasRect.right))
			Instrument::x_startdisplay = (width - Instrument::canvasRect.right) / Instrument::ZoomCoefficient;
		if (height - Instrument::y_startdisplay*Instrument::ZoomCoefficient < (Instrument::canvasRect.bottom))
			Instrument::y_startdisplay = (height - Instrument::canvasRect.bottom) / Instrument::ZoomCoefficient;
		UpdateWindow(hwnd);
		DisplayMemoryDC();
	}
}

void ZoomAndMove::Move(HWND hwnd, HINSTANCE hinstance, int dx, int dy)
{
	int height = realHeight * Instrument::ZoomCoefficient;
	int width = realWidth * Instrument::ZoomCoefficient;
	if (((dx + Instrument::x_startdisplay) > 0) && ((dx + Instrument::x_startdisplay)*Instrument::ZoomCoefficient < (width - Instrument::canvasRect.right)))
		Instrument::x_startdisplay += dx;
	if (((dy + Instrument::y_startdisplay) > 0) && ((dy + Instrument::y_startdisplay)*Instrument::ZoomCoefficient < (height - Instrument::canvasRect.bottom)))
		Instrument::y_startdisplay += dy;
	DisplayMemoryDC();
}