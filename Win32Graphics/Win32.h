#pragma once
#include <Windows.h>
class Win32
{
private:
	static COLORREF* bmp;
public:
	static bool showColorDialog(HWND hwnd, COLORREF& choice);
	static void StartFastPixel(HWND hwnd, HDC hdc);
	static void SetPixelFast(HDC hdc, int x, int y, COLORREF color);
	static void EndFastPixel(HDC hdc);

	static HDC BeginDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps);
	static void EndDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps);
};

