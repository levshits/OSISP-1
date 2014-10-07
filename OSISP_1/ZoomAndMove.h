#pragma once
class ZoomAndMove
{
public:
	static int realWidth;
	static int realHeight;
	static void CreateCanvas(HWND hwnd, HINSTANCE hinstance, int width, int height);
	static void UpdateCanvas(HWND hwnd, HINSTANCE hinstance);
	static void DisplayMemoryDC();
	static void Zoom(HWND hwnd, HINSTANCE hinstance, double value);
	static void Move(HWND hwnd, HINSTANCE hinstance, int dx, int dy);

};

