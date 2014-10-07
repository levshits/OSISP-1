#include "windows.h"

#pragma once
#define UI_INSTRUMENTS_PEN  1000
#define UI_INSTRUMENTS_LINE  1001
#define UI_INSTRUMENTS_POLYGON  1003
#define UI_INSTRUMENTS_OVAL  1004
#define UI_INSTRUMENTS_TEXTOUT  1006
#define UI_INSTRUMENTS_GRABE  1007
#define UI_INSTRUMENTS_FONT  1008
#define UI_INSTRUMENTS_PENCOLOR 1009
#define UI_INSTRUMENTS_BRUSHCOLOR 1010


#define UI_EDIT_WIDTH 3000
#define TOOLBAR_HEIGHT 30
#define CANVAS_OFFSET_X 5
class UI
{
 public:
	UI(){};
	static void CreateUI(HWND hWnd, HINSTANCE hInstance);
	static HWND linewidthLabel;
	static HWND linewidthEdit;
	static int CanvasOffsetX;
	static int CanvasOffsetY;
	static int getCanvasCursorX(int x)
	{
		return x - CanvasOffsetX;
	}
	static int getCanvasCursorY(int y)
	{
		return y - CanvasOffsetY;
	}
	static void CreateCanvas(HWND hwnd, HINSTANCE hinstance, int width, int height);
	static void CreateCanvasMemoryDC(HWND hwnd, HINSTANCE hinstance, int width, int height);
	static bool isPointInsideCanvas(int x, int y);
};

