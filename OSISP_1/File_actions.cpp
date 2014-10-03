#include "stdafx.h"
#include <commdlg.h>

void SaveBitmapDialog(HWND hWnd, TCHAR fileName[])
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = _T("Bitmap file (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = fileName;
	ofn.lpstrDefExt = _T("bmp");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Save as Bitmap");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	GetSaveFileName(&ofn);

}

void OpenBitmapDialog(HWND hWnd, TCHAR fileName[])
{
	OPENFILENAME ofn;	
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = _T("Bitmap file (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = _T("bmp");

	GetOpenFileName(&ofn);
	
}

PBITMAPINFO CreateBitmapInfo(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;

	// Retrieve the bitmap color format, width, and height.  
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
	{
		//errhandler("GetObject", hwnd);
	}

	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)* (1 << cClrBits));

	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.  

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}

void CreateBitmapFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
	{
		//errhandler("GlobalAlloc", hwnd);
	}


	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		//errhandler("GetDIBits", hwnd);
	}

	// Create the .BMP file.  
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hf == INVALID_HANDLE_VALUE)
	{
		//errhandler("CreateFile", hwnd);
	}


	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
	// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER)+
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD)+pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)+
		pbih->biSize + pbih->biClrUsed
		* sizeof (RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.  
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		//errhandler("WriteFile", hwnd);
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+pbih->biClrUsed * sizeof (RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
	{
		//errhandler("WriteFile", hwnd);
	}

	// Copy the array of color indices into the .BMP file.  
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
	{
		//errhandler("WriteFile", hwnd);
	}


	// Close the .BMP file.  
	if (!CloseHandle(hf))
	{
		//errhandler("CloseHandle", hwnd);
	}


	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}

void CopyBitmap(HBITMAP hbmOn, HBITMAP hbmWhat)
{
	HDC hdcSrc = CreateCompatibleDC(NULL);
	HDC hdcDst = CreateCompatibleDC(NULL);
	BITMAP bm;
	SelectObject(hdcSrc, hbmOn);
	SelectObject(hdcDst, hbmWhat);
	GetObject(hbmWhat, sizeof(bm), &bm);
	BitBlt(hdcDst, 20, 5, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
	DeleteDC(hdcSrc);
	DeleteDC(hdcDst);
}

void SaveImageToBitmap(HWND hWnd, LPTSTR filePath)
{
	PBITMAPINFO pBmpInfo =
		CreateBitmapInfo(hWnd, Instrument::Buffer);

	CreateBitmapFile(hWnd, filePath, pBmpInfo, Instrument::Buffer, Instrument::MemoryDC);
}

void LoadBitmapFromFile(HWND hWnd, LPTSTR fileName)
{

	/*
	HBITMAP loadedBitmap = (HBITMAP)LoadImage(NULL, L"d:\\my.bmp",
	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CopyBitmap(Instrument::Buffer, loadedBitmap);
	SelectObject(Instrument::MemoryDC, Instrument::Buffer);
	BitBlt(Instrument::DeviceDC, Instrument::canvasRect.left, Instrument::canvasRect.top, Instrument::canvasRect.right,
	Instrument::canvasRect.bottom, Instrument::MemoryDC, Instrument::canvasRect.left, Instrument::canvasRect.top, SRCCOPY);
		*/	

	Instrument::Buffer = (HBITMAP)LoadImage(NULL, fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(Instrument::MemoryDC, Instrument::Buffer);
	BitBlt(Instrument::DeviceDC, Instrument::canvasRect.left, Instrument::canvasRect.top, Instrument::canvasRect.right,
		Instrument::canvasRect.bottom, Instrument::MemoryDC, Instrument::canvasRect.left, Instrument::canvasRect.top, SRCCOPY);
	
}