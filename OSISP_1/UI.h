#include "windows.h"

#pragma once
#define UI_INSTRUMENTS_PEN  1000
#define UI_INSTRUMENTS_LINE  1001
#define UI_INSTRUMENTS_POLYGON  1003
#define UI_INSTRUMENTS_OVAL  1004
#define UI_INSTRUMENTS_TRIANGLE  1005
#define UI_INSTRUMENTS_TEXTOUT  1006
#define UI_INSTRUMENTS_GRABE  1007
#define UI_INSTRUMENTS_ZOOM  1008
#define UI_INSTRUMENTS_PENCOLOR 1009
#define UI_INSTRUMENTS_BRUSHCOLOR 1010

#define UI_MODIFICATORS_LINEWIDTH  2000
#define UI_MODIFICATORS_PENCOLOR  2001
#define UI_MODIFICATORS_BRUSHCOLOR  2003


#define UI_EDIT_WIDTH 3000
class UI
{
 public:
	UI(){};
	void CreateUI(HWND hWnd, HINSTANCE hInstance);
	HWND linewidthLabel;
	HWND linewidthEdit;
	DWORD penColor;
	DWORD brushColor;
	int CanvasOffsetX;
	int CanvasOffsetY;

	int getCanvasCursorX(int x)
	{
		return x - CanvasOffsetX;
	}

	int getCanvasCursorY(int y)
	{
		return y - CanvasOffsetY;
	}

private:
	void CreateCanvas(HWND hwnd, HINSTANCE hinstance, int width, int height);
	
};

