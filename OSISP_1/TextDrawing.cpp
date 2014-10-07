#include "stdafx.h"
#include "TextDrawing.h"
#include "string.h"


void TextDrawing::Draw(int x, int y)
{
	RECT rect = {ToMemoryDCX(previous_x), ToMemoryDCY(previous_y), ToMemoryDCX(x), ToMemoryDCY(y)};
	DrawText(MemoryDC, text, _tcslen(text), &rect, DT_CENTER);
	ZoomAndMove::DisplayMemoryDC();
}

void TextDrawing::Display(int x, int y)
{
	
	LOGFONT temp = *logFont;
	temp.lfHeight *= ZoomCoefficient;
	temp.lfWidth *= ZoomCoefficient;
	HFONT hfont = CreateFontIndirect(&temp);
	HFONT hfontPrev = (HFONT)SelectObject(DeviceDC, hfont);
	DeleteObject(hfontPrev);

	ZoomAndMove::DisplayMemoryDC();
	RECT rect = { previous_x, previous_y, x, y };
	DrawText(DeviceDC, text, _tcslen(text), &rect, DT_CENTER);
	
}