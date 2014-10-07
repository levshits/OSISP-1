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
#include "TextDrawing.h"
#include "Grabe.h"

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
INT_PTR CALLBACK CreateDlgProc(HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK InputDialog(HWND, UINT, WPARAM, LPARAM);

HDC DisplayDC;
HWND ButtonPen;
int previous_x, previous_y;
bool isActivated = false;
HWND Instrument::Canvas = 0;
HDC Instrument::DeviceDC = 0;
HDC Instrument::MemoryDC = 0;
HBITMAP Instrument::Buffer = 0;
RECT Instrument::canvasRect = { 0, 0, 0, 0 };
DWORD Instrument::BrushColor = 0;
DWORD Instrument::PenColor = 0;
int UI::CanvasOffsetX = CANVAS_OFFSET_X;
int UI::CanvasOffsetY = TOOLBAR_HEIGHT;
HWND UI::linewidthEdit = 0;
HWND UI::linewidthLabel = 0;
double Instrument::ZoomCoefficient = 0;
int Instrument::x_startdisplay = 0;
int Instrument::y_startdisplay = 0;
int Instrument::Width = 1;
int ZoomAndMove::realWidth = 0;
int ZoomAndMove::realHeight = 0;
Instrument * instrument = Pen::GetInstance();
HWND hwnd;
HINSTANCE hinstance;
TCHAR TextDrawing::text[] = L" ";
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
	hinstance = hInstance;

	hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CAPTION,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	hwnd = hWnd;
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);
	UI::CreateUI(hWnd, hInst);
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

void FontChooseDialog(HWND hwnd, HDC hdc, HDC hdc1)
{
	CHOOSEFONT cf;            // common dialog box structure
	static LOGFONT lf;        // logical font structure
	static DWORD rgbCurrent;  // current text color
	HFONT hfont, hfontPrev;
	DWORD rgbPrev;

	// Initialize CHOOSEFONT
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof (cf);
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.rgbColors = rgbCurrent;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

	if (ChooseFont(&cf) == TRUE)
	{
		hfont = CreateFontIndirect(cf.lpLogFont);
		hfontPrev = (HFONT)SelectObject(hdc, hfont);
		DeleteObject(hfontPrev);
		hfontPrev = (HFONT)SelectObject(hdc1, hfont);
		DeleteObject(hfontPrev);
		rgbCurrent = cf.rgbColors;
		rgbPrev = SetTextColor(hdc, rgbCurrent);
		rgbPrev = SetTextColor(hdc1, rgbCurrent);

	}
}

LRESULT ProcessEditNotification(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
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

	switch (wmId)
	{
	case UI_INSTRUMENTS_PEN:
		instrument = Pen::GetInstance();
		break;
	case UI_INSTRUMENTS_GRABE:
		instrument = Grabe::GetInstance();
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
		HPEN newPen = CreatePen(NULL, Instrument::Width, color);
		HGDIOBJ temp = SelectObject(Instrument::DeviceDC, newPen);
		DeleteObject(temp);
		temp = SelectObject(Instrument::MemoryDC, newPen);
		DeleteObject(temp);
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
	case UI_INSTRUMENTS_TEXTOUT:
	{
								   instrument = TextDrawing::GetInstance();
								   DialogBox(hInst, MAKEINTRESOURCE(INPUT_TEXT_DIALOG),
									   hWnd, InputDialog);
	}
		break;
	case UI_INSTRUMENTS_FONT:
	{
		FontChooseDialog(hWnd,Instrument::DeviceDC, Instrument::MemoryDC);
	}
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
		LoadBitmapFromFile(hWnd,hinstance, fileName);

	}
		break;
	case IDM_SAVE:
	{
		TCHAR fileName[_MAX_PATH] = { 0 };
		SaveBitmapDialog(hWnd, fileName);
		SaveImageToBitmap(hWnd, fileName);
	}
		break;
	case IDM_CREATE:
	{
		DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATE_DIALOG),
			hWnd, CreateDlgProc);


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
		
		int x = UI::getCanvasCursorX(LOWORD(lParam));
		int y = UI::getCanvasCursorY(HIWORD(lParam));
		if (UI::isPointInsideCanvas(x, y))
		{
			instrument->Initialize(x,y);
			isActivated = true;
		}
	}
		break;
	case WM_MOUSEMOVE:
	{
		if (isActivated)
		{
			int x = UI::getCanvasCursorX(LOWORD(lParam));
			int y = UI::getCanvasCursorY(HIWORD(lParam));
			if (UI::isPointInsideCanvas(x, y))
			{
				instrument->Display(x, y);
			}
		}
	}
		break;
	case WM_LBUTTONUP:
	{
		if (isActivated)
		{
			instrument->Draw(
				UI::getCanvasCursorX(LOWORD(lParam)),
				UI::getCanvasCursorY(HIWORD(lParam)));
			isActivated = false;
		}		
	}
		break;
	case WM_MOUSEWHEEL:
	{
		switch (GET_KEYSTATE_WPARAM(wParam))
		{
		case MK_CONTROL:
			ZoomAndMove::Zoom(hwnd, hinstance, GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		case MK_SHIFT:
			ZoomAndMove::Move(hwnd, hinstance, GET_WHEEL_DELTA_WPARAM(wParam) / 12, 0);
			break;
		default:
			ZoomAndMove::Move(hwnd, hinstance, 0, GET_WHEEL_DELTA_WPARAM(wParam) / 12);
		}

	}
		break;
	case WM_SIZE:
	{
					ZoomAndMove::UpdateCanvas(hwnd, hinstance);
					ZoomAndMove::DisplayMemoryDC();
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
		ProcessDragRequest(hWnd, hinstance, (HDROP)wParam);
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

INT_PTR CALLBACK CreateDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			TCHAR bitmapWidthStr[MAX_LOADSTRING];
			GetDlgItemText(hDlg, IDC_BITMAP_WIDTH, bitmapWidthStr, MAX_LOADSTRING);

			TCHAR bitmapHeightStr[MAX_LOADSTRING];
			GetDlgItemText(hDlg, IDC_BITMAP_HEIGHT, bitmapHeightStr, MAX_LOADSTRING);

			int bitmapWidth = _wtoi(bitmapWidthStr);
			int bitmapHeight = _wtoi(bitmapHeightStr);

			if ((bitmapWidth == 0) || (bitmapHeight == 0))
			{
				MessageBox(hDlg, L"Sorry, incorrect value specified.", L"Create Bitmap", MB_OK);
			} 
			else
			{
				
				/*ScrollWindowEx(instrument->Canvas, 10, 10, NULL, &Instrument::canvasRect,
					NULL, NULL, NULL);*/
				//SetWindowPos(instrument->Canvas, HWND_TOPMOST, 130, 30,
				//	bitmapWidth, bitmapHeight, SWP_SHOWWINDOW);

				ZoomAndMove::CreateCanvas(hwnd, hinstance, bitmapWidth, bitmapHeight);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}	
		}
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;			
		}
	}
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK InputDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
					   switch (LOWORD(wParam))
					   {
					   case IDOK:
					   {
									
									GetDlgItemText(hDlg, IDC_BITMAP_WIDTH, TextDrawing::text, MAX_LOADSTRING);

									EndDialog(hDlg, LOWORD(wParam));
									return (INT_PTR)TRUE;

					   }
						   break;
					   case IDCANCEL:
						   EndDialog(hDlg, LOWORD(wParam));
						   return (INT_PTR)TRUE;
					   }
	}
	}
	
	return (INT_PTR)FALSE;
}
