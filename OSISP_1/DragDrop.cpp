#include "stdafx.h"
#include "DragDrop.h"
#include "File_actions.h"

void ProcessDragRequest(HWND hWnd, HDROP hDrop) {
	UINT fileCount = DragQueryFile(hDrop, -1, NULL, NULL);

	if (fileCount == 1) {
		TCHAR fileName[MAX_PATH] = { 0 };
		DragQueryFile(hDrop, 0, fileName, MAX_PATH);
		if (MessageBox(hWnd, L"Close current file?", L"Drag and Drop", MB_YESNO) == IDYES)
		{
			SHFILEINFO shfi;
			SHGetFileInfo(
				fileName,
				0,
				&shfi,
				sizeof(SHFILEINFO),
				SHGFI_TYPENAME
				);
			if (_tccmp(shfi.szTypeName, L"Точечный рисунок") == 0) 
			{
				LoadBitmapFromFile(hWnd, fileName);
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
