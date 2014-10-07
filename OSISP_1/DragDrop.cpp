#include "stdafx.h"
#include "DragDrop.h"
#include "File_actions.h"

BOOL validateFileType(TCHAR fileName[])
{
	UINT length = _tcslen(fileName);
	TCHAR extension[5];
	_tcsnccpy_s(extension, 5, &fileName[length-4], 4);

	return  (_tcscmp(_T(".bmp"), extension) == 0);
}

void ProcessDragRequest(HWND hWnd, HINSTANCE hinstance,HDROP hDrop) {
	UINT fileCount = DragQueryFile(hDrop, -1, NULL, NULL);

	if (fileCount == 1) {
		TCHAR fileName[MAX_PATH] = { 0 };
		DragQueryFile(hDrop, 0, fileName, MAX_PATH);
		if (MessageBox(hWnd, L"Close current file?", L"Drag and Drop", MB_YESNO) == IDYES)
		{		
			
			if (validateFileType(fileName)) 
			{
				LoadBitmapFromFile(hWnd, hinstance, fileName);
			}
			else
			{
				MessageBox(hWnd, L"Inapproptiate file format.", L"Drag and Drop", MB_OK);
			}	
		}
	}
	else if (fileCount > 1) {
		MessageBox(hWnd, L"Please, drop only one file at a time.", L"Drag and Drop", MB_OK);
	}
	DragFinish(hDrop);
	
}
