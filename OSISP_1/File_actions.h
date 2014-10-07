#pragma once
#include "stdafx.h"
#include <Commdlg.h>

void SaveBitmapDialog(HWND hWnd, TCHAR fileName[]);

void OpenBitmapDialog(HWND hWnd, TCHAR fileName[]);

void CopyBitmap(HBITMAP hbmOn, HBITMAP hbmWhat);

void SaveImageToBitmap(HWND hWnd, LPTSTR filePath);

void LoadBitmapFromFile(HWND hWnd, HINSTANCE hinstance, LPTSTR fileName);
