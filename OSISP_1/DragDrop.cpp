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
			LoadBitmapFromFile(hWnd, fileName);
		}
	}
	else if (fileCount > 1) {
		MessageBox(hWnd, L"Please, drop only one file at a time.", L"Drag and Drop", MB_OK);
	}
	DragFinish(hDrop);
}
