#include "stdafx.h"
#include "UI.h"
#include "windows.h"
#include <Commctrl.h>
#include <shellapi.h>
#include <WinUser.h>
#include "Instrument.h"
#include "Resource.h"
#pragma comment(lib, "COMCTL32.lib")

void UI::CreateUI(HWND hWnd, HINSTANCE hInstance)
{
	#define BUTTON_WIDTH  110
	#define BUTTON_HEIGHT 30

	//Line width edit parameters
	#define EDIT_WIDTH 30
	#define EDIT_HEIGHT 20
	#define EDIT_HMARGIN 375
	#define EDIT_VMARGIN 4

	//Line width label parameters
	#define LABEL_WIDTH 70
	#define LABEL_HEIGHT 20
	#define LABEL_HMARGIN 300
	#define LABEL_VMARGIN 4

	#define DEFAULT_LINE_WIDTH L"1"
	#define LINEWIDTH_LABEL_TXT L"Line width"
	
	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);
	const int NUMBUTTONS = 12;
	TBBUTTON tbrButtons[NUMBUTTONS];

	//Pen
	tbrButtons[0].iBitmap = 0;
	tbrButtons[0].idCommand = UI_INSTRUMENTS_PEN;
	tbrButtons[0].fsState = TBSTATE_ENABLED;
	tbrButtons[0].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[0].dwData = 0L;
	tbrButtons[0].iString = 0;

	//Line
	tbrButtons[1].iBitmap = 3;
	tbrButtons[1].idCommand = UI_INSTRUMENTS_LINE;
	tbrButtons[1].fsState = TBSTATE_ENABLED;
	tbrButtons[1].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[1].dwData = 0L;
	tbrButtons[1].iString = 0;

	//Polygon
	tbrButtons[2].iBitmap = 1;
	tbrButtons[2].idCommand = UI_INSTRUMENTS_POLYGON;
	tbrButtons[2].fsState = TBSTATE_ENABLED;
	tbrButtons[2].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[2].dwData = 0L;
	tbrButtons[2].iString = 0;

	//Oval
	tbrButtons[3].iBitmap = 2;
	tbrButtons[3].idCommand = UI_INSTRUMENTS_OVAL;
	tbrButtons[3].fsState = TBSTATE_ENABLED;
	tbrButtons[3].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[3].dwData = 0L;
	tbrButtons[3].iString = 0;

	//Triangle
	tbrButtons[4].iBitmap = 4;
	tbrButtons[4].idCommand = UI_INSTRUMENTS_FONT;
	tbrButtons[4].fsState = TBSTATE_ENABLED;
	tbrButtons[4].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[4].dwData = 0L;
	tbrButtons[4].iString = 0;

	//Text output
	tbrButtons[5].iBitmap = 6;
	tbrButtons[5].idCommand = UI_INSTRUMENTS_TEXTOUT;
	tbrButtons[5].fsState = TBSTATE_ENABLED;
	tbrButtons[5].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[5].dwData = 0L;
	tbrButtons[5].iString = 0;

	//Group separator
	tbrButtons[6].iBitmap = 0;
	tbrButtons[6].idCommand = 0;
	tbrButtons[6].fsState = TBSTATE_ENABLED;
	tbrButtons[6].fsStyle = TBSTYLE_SEP;
	tbrButtons[6].dwData = 0L;
	tbrButtons[6].iString = 0;

	//Pen color selector
	tbrButtons[7].iBitmap = 9;
	tbrButtons[7].idCommand = UI_INSTRUMENTS_PENCOLOR;
	tbrButtons[7].fsState = TBSTATE_ENABLED;
	tbrButtons[7].fsStyle = TBSTYLE_BUTTON;
	tbrButtons[7].dwData = 0L;
	tbrButtons[7].iString = 0;

	//brush color selector
	tbrButtons[8].iBitmap = 10;
	tbrButtons[8].idCommand = UI_INSTRUMENTS_BRUSHCOLOR;
	tbrButtons[8].fsState = TBSTATE_ENABLED;
	tbrButtons[8].fsStyle = TBSTYLE_BUTTON;
	tbrButtons[8].dwData = 0L;
	tbrButtons[8].iString = 0;

	//Group separator
	tbrButtons[9].iBitmap = 0;
	tbrButtons[9].idCommand = 0;
	tbrButtons[9].fsState = TBSTATE_ENABLED;
	tbrButtons[9].fsStyle = TBSTYLE_SEP;
	tbrButtons[9].dwData = 0L;
	tbrButtons[9].iString = 0;

	//Grabe
	tbrButtons[10].iBitmap = 5;
	tbrButtons[10].idCommand = UI_INSTRUMENTS_GRABE;
	tbrButtons[10].fsState = TBSTATE_ENABLED;
	tbrButtons[10].fsStyle = TBSTYLE_BUTTON;
	tbrButtons[10].dwData = 0L;
	tbrButtons[10].iString = 0;

	//Group separator
	tbrButtons[11].iBitmap = 0;
	tbrButtons[11].idCommand = 0;
	tbrButtons[11].fsState = TBSTATE_ENABLED;
	tbrButtons[11].fsStyle = TBSTYLE_SEP;
	tbrButtons[11].dwData = 0L;
	tbrButtons[11].iString = 0;


	


	HWND hWndToolbar = CreateToolbarEx(hWnd,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		IDB_INSTRUMENTS, NUMBUTTONS,
		hInstance, IDB_INSTRUMENTS,
		tbrButtons, NUMBUTTONS,
		16, 16, 16, 16,
		sizeof(TBBUTTON));
	
	linewidthEdit = CreateWindowEx(NULL, L"EDIT", DEFAULT_LINE_WIDTH,
		WS_VISIBLE | WS_CHILD | WS_EX_CLIENTEDGE | WS_BORDER,
		EDIT_HMARGIN, EDIT_VMARGIN, 
		EDIT_WIDTH, EDIT_HEIGHT,
		hWnd, (HMENU)UI_EDIT_WIDTH, hInstance, NULL);

	SetParent(linewidthLabel, hWndToolbar);

	linewidthLabel = CreateWindowEx(NULL, L"STATIC", LINEWIDTH_LABEL_TXT,
		WS_VISIBLE | WS_CHILD | WS_EX_CLIENTEDGE,
		LABEL_HMARGIN, LABEL_VMARGIN,
		LABEL_WIDTH, LABEL_HEIGHT,
		hWnd, NULL, hInstance, NULL);

	SetParent(linewidthEdit, hWndToolbar);		
}

void UI::CreateCanvas(HWND hWnd, HINSTANCE hInstance, int width, int height)
{
	RECT rect = { 0, 0, 0, 0 };
	GetClientRect(hWnd, &rect);
	CanvasOffsetX = CANVAS_OFFSET_X;
	CanvasOffsetY = TOOLBAR_HEIGHT;
	if ((width == 0) && (height == 0))
	{		
		width = rect.right - 2 * CanvasOffsetX;
		height = rect.bottom - CanvasOffsetY;
	}
	else
	{
		CanvasOffsetX = (rect.right - 2 * CanvasOffsetX - width)/ 2;
		CanvasOffsetY = (rect.bottom - CanvasOffsetY - height)/2 + CanvasOffsetY;
	}
	HWND Canvas = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES,
		L"Static",
		NULL,
		WS_CHILD | WS_VISIBLE,
		CanvasOffsetX, CanvasOffsetY, width , height,
		hWnd, NULL, hInstance, NULL);
	DragAcceptFiles(Canvas, FALSE);
	UpdateWindow(hWnd);
	UpdateWindow(Canvas);
	rect = { 0, 0, width-4, height-4 };
	if (Instrument::Canvas != 0)
		DestroyWindow(Instrument::Canvas);
	Instrument::Canvas = Canvas;
	Instrument::canvasRect = rect;
	if (Instrument::DeviceDC != 0)
		DeleteObject(Instrument::Canvas);
	Instrument::DeviceDC = GetDC(Canvas);

	
}

void UI::CreateCanvasMemoryDC(HWND hwnd, HINSTANCE hinstance, int width, int height)
{
	RECT rect = { 0, 0, width, height };
	Instrument::MemoryDC = CreateCompatibleDC(Instrument::DeviceDC);
	Instrument::Buffer = CreateCompatibleBitmap(Instrument::DeviceDC, width, height);
	SelectObject(Instrument::MemoryDC, Instrument::Buffer);
	static HBRUSH Brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(Instrument::MemoryDC, &rect, Brush);
}

bool UI::isPointInsideCanvas(int x, int y)
{
	if ((x<Instrument::canvasRect.right) && (x>0) && (y<Instrument::canvasRect.bottom) && (y>0))
		return true;
	else
		return false;

}