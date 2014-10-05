#include "stdafx.h"
#include "UI.h"
#include "windows.h"
#include <Commctrl.h>
#include <shellapi.h>
#include <WinUser.h>
#include "Instrument.h"
#include "Resource.h"
#pragma comment(lib, "COMCTL32.lib")
UI::UI(HWND hWnd, HINSTANCE hInstance)
{
#define BUTTON_WIDTH  110
#define BUTTON_HEIGHT 30
	int margin = 100;
	
	HWND penButton = CreateWindowEx(NULL,L"BUTTON",L"PEN",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_INSTRUMENTS_PEN, hInstance, NULL);
	margin += BUTTON_HEIGHT;
	HWND brushButton = CreateWindowEx(NULL, L"BUTTON", L"LINE",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_INSTRUMENTS_LINE, hInstance, NULL);
	margin += BUTTON_HEIGHT;
	HWND PolygonButton = CreateWindowEx(NULL, L"BUTTON", L"Polygon",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_INSTRUMENTS_POLYGON, hInstance, NULL);
	margin += BUTTON_HEIGHT;
	HWND ovalButton = CreateWindowEx(NULL, L"BUTTON", L"OVAL",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_INSTRUMENTS_OVAL, hInstance, NULL);
	margin += BUTTON_HEIGHT;

	HWND penColorButton = CreateWindowEx(NULL, L"BUTTON", L"PEN COLOR",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_INSTRUMENTS_PENCOLOR, hInstance, NULL);
	margin += BUTTON_HEIGHT;
	HWND brushColorButton = CreateWindowEx(NULL, L"BUTTON", L"BRUSH COLOR",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_INSTRUMENTS_BRUSHCOLOR, hInstance, NULL);
	margin += BUTTON_HEIGHT;
	linewidthEdit = CreateWindowEx(NULL, L"EDIT", NULL,
		WS_VISIBLE | WS_CHILD, 10, margin, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)UI_SPINBOX_WIDTH, hInstance, NULL);
	RECT rect = { 0, 0, 0, 0 };
	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);
	const int NUMBUTTONS = 13;
	TBBUTTON tbrButtons[NUMBUTTONS];

	//Pen
	tbrButtons[0].iBitmap = 0;
	tbrButtons[0].idCommand = UI_INSTRUMENTS_PEN;
	tbrButtons[0].fsState = TBSTATE_ENABLED | TBSTATE_CHECKED;
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
	tbrButtons[4].idCommand = UI_INSTRUMENTS_TRIANGLE;
	tbrButtons[4].fsState = TBSTATE_ENABLED;
	tbrButtons[4].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP;
	tbrButtons[4].dwData = 0L;
	tbrButtons[4].iString = 0;

	//Text output
	tbrButtons[5].iBitmap = 6;
	tbrButtons[5].idCommand = UI_INSTRUMENTS_TEXTOUT;
	tbrButtons[5].fsState = TBSTATE_ENABLED;
	tbrButtons[5].fsStyle = TBSTYLE_BUTTON;
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

	//Group separator
	tbrButtons[8].iBitmap = 0;
	tbrButtons[8].idCommand = 0;
	tbrButtons[8].fsState = TBSTATE_ENABLED;
	tbrButtons[8].fsStyle = TBSTYLE_SEP;
	tbrButtons[8].dwData = 0L;
	tbrButtons[8].iString = 0;

	//Grabe
	tbrButtons[9].iBitmap = 5;
	tbrButtons[9].idCommand = UI_INSTRUMENTS_GRABE;
	tbrButtons[9].fsState = TBSTATE_ENABLED;
	tbrButtons[9].fsStyle = TBSTYLE_BUTTON;
	tbrButtons[9].dwData = 0L;
	tbrButtons[9].iString = 0;

	//Group separator
	tbrButtons[10].iBitmap = 0;
	tbrButtons[10].idCommand = 0;
	tbrButtons[10].fsState = TBSTATE_ENABLED;
	tbrButtons[10].fsStyle = TBSTYLE_SEP;
	tbrButtons[10].dwData = 0L;
	tbrButtons[10].iString = 0;

	//Zoom
	tbrButtons[11].iBitmap = 7;
	tbrButtons[11].idCommand = UI_INSTRUMENTS_ZOOM;
	tbrButtons[11].fsState = TBSTATE_ENABLED;
	tbrButtons[11].fsStyle = TBSTYLE_BUTTON;
	tbrButtons[11].dwData = 0L;
	tbrButtons[11].iString = 0;
	

	//Group separator
	tbrButtons[12].iBitmap = 0;
	tbrButtons[12].idCommand = 0;
	tbrButtons[12].fsState = TBSTATE_ENABLED;
	tbrButtons[12].fsStyle = TBSTYLE_SEP;
	tbrButtons[12].dwData = 0L;
	tbrButtons[12].iString = 0;

	
	HWND hWndToolbar = CreateToolbarEx(hWnd,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		IDB_INSTRUMENTS,
		NUMBUTTONS,
		hInstance,
		IDB_INSTRUMENTS,
		tbrButtons,
		NUMBUTTONS,
		16, 16, 16, 16,
		sizeof(TBBUTTON));
	GetClientRect(hWnd, &rect);

	HWND Canvas = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES,
									L"Static", 
									NULL, 
									WS_CHILD | WS_VISIBLE, 
									130, 30, rect.right - 150, rect.bottom-30, 
									hWnd, NULL, hInstance, NULL);

	
	DragAcceptFiles(Canvas, FALSE);
	UpdateWindow(Canvas);

	GetClientRect(Canvas, &rect);
	Instrument::canvasRect = rect;
	Instrument::DeviceDC = GetDC(Canvas);
	Instrument::MemoryDC = CreateCompatibleDC(Instrument::DeviceDC);
	Instrument::Buffer = CreateCompatibleBitmap(Instrument::DeviceDC, rect.right, rect.bottom);
	SelectObject(Instrument::MemoryDC, Instrument::Buffer);
	static HBRUSH Brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(Instrument::MemoryDC, &rect, Brush);
}

