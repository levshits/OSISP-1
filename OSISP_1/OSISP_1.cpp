// OSISP_1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OSISP_1.h"
#include "UI.h"
#include <Commdlg.h>
#include <CommCtrl.h>
#include <WinUser.h>
#include <shellapi.h>
#include "File_actions.h"
#include "DragDrop.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
WNDPROC wpOrigEditProc;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OSISP_1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISP_1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP_1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_OSISP_1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CAPTION,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);
	UI::UI(hWnd, hInst);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
HDC DisplayDC;
HWND ButtonPen;
int previous_x, previous_y;
bool isActivated = false;
HDC Instrument::DeviceDC = 0;
HDC Instrument::MemoryDC = 0;
HBITMAP Instrument::Buffer = 0;
RECT Instrument::canvasRect = { 0, 0, 0, 0 };
DWORD Instrument::BrushColor = 0;
DWORD Instrument::PenColor = 0;
Instrument * instrument = Pen::GetInstance();

DWORD ColorChooseDialog(HWND hWnd)
{
	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[3]; // array of custom colors             
	static DWORD rgbCurrent;        // initial color selection

	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		rgbCurrent = cc.rgbResult;
		return cc.rgbResult;
	}
	return 0;
}

LRESULT ProcessEditNotification(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	HWND edit = (HWND)lParam;

	switch (wmEvent)
	{
	case EN_CHANGE:
	{
		TCHAR lineWidth[5] = { 0 };

		GetWindowText(edit, lineWidth, 5);
		int width = _wtoi(lineWidth);

		if (width == 0) {
			MessageBox(hWnd, L"Sorry, incorrect value specified.", L"Incorrect value", MB_OK);
			return 0;
		}

		instrument->Width = width;
		HPEN newPen = CreatePen(NULL, instrument->Width, Instrument::PenColor);
		SelectObject(Instrument::DeviceDC, newPen);
		SelectObject(Instrument::MemoryDC, newPen);
		return 0;
	}
		break;
	default:
		return 0;
	}
}

LRESULT ProcessCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	wmId = LOWORD(wParam);
	wmEvent = HIWORD(wParam);

	switch (wmId)
	{
	case UI_INSTRUMENTS_PEN:
		instrument = Pen::GetInstance();
		break;
	case UI_INSTRUMENTS_LINE:
		instrument = Line::GetInstance();
		break;
	case UI_INSTRUMENTS_POLYGON:
		instrument = Polygon::GetInstance();
		break;
	case UI_INSTRUMENTS_PENCOLOR:
	{
		DWORD color = ColorChooseDialog(hWnd);
		Instrument::PenColor = color;
		HPEN newPen = CreatePen(NULL, 1, color);
		SelectObject(Instrument::DeviceDC, newPen);
		SelectObject(Instrument::MemoryDC, newPen);
	}
		break;
	case UI_INSTRUMENTS_BRUSHCOLOR:
	{
		DWORD color = ColorChooseDialog(hWnd);
		Instrument::BrushColor = color;
		HBRUSH newBrush = CreateSolidBrush(color);
		SelectObject(Instrument::DeviceDC, newBrush);
		SelectObject(Instrument::MemoryDC, newBrush);
	}
		break;
	case UI_INSTRUMENTS_OVAL:
		instrument = Oval::GetInstance();
		break;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDM_OPEN:
	{
		TCHAR fileName[MAX_PATH] = { 0 };
		OpenBitmapDialog(hWnd, fileName);
		LoadBitmapFromFile(hWnd, fileName);

	}
		break;
	case IDM_SAVE:
	{
		TCHAR fileName[_MAX_PATH] = { 0 };
		SaveBitmapDialog(hWnd, fileName);
		SaveImageToBitmap(hWnd, fileName);
	}
		break;
	case UI_EDIT_WIDTH:
	{
		ProcessEditNotification(hWnd, wParam, lParam);							 
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		instrument->Initialize(LOWORD(lParam) - 130, HIWORD(lParam) - 30);
		isActivated = true;
	}
		break;
	case WM_MOUSEMOVE:
	{
		if (isActivated)
		{
			instrument->Display(LOWORD(lParam) - 130, HIWORD(lParam) - 30);
		}
	}
		break;
	case WM_LBUTTONUP:
	{
		instrument->Draw(LOWORD(lParam) - 130, HIWORD(lParam) - 30);
		isActivated = false;
	}
		break;	
	case WM_COMMAND:
		ProcessCommand(hWnd, message, wParam, lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
	{
		if (MessageBox(hWnd, L"Would you like to quit?", L"Quit", MB_YESNO) == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else {
			return 0;
		}

	}
		break;
	case  WM_DROPFILES:
	{
		ProcessDragRequest(hWnd, (HDROP)wParam);
		return 0;
	}	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

