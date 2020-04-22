#pragma once
#include <Windows.h>
class Win32
{
public:
	static bool showColorDialog(HWND hwnd, COLORREF& choice);
	static void StartFastPixel(HDC hdc);
	static void SetPixelFast(HDC hdc, int x, int y, COLORREF color);
	static COLORREF GetPixelFast(HDC hdc, int x, int y);
	static void EndFastPixel(HDC hdc);

	//DoubleBuffer is not compatible with hbrBackground in WndClass.
	//If you decide to use double buffer, you must disable it by setting
	//hbrBackground to NULL during the initialization of the window.
	static HDC BeginDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps);
	static void EndDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps);

	static HDC cloneHDC(HDC Orghdc);

	static void getScreenResolution(HDC hdc, int& width, int& height);

	static HWND CreateFunctionalWindow(HINSTANCE hInstance, WNDPROC lpfnWndProc, RECT window, LPCWSTR windowTitle, LPCWSTR menu = NULL);

	static void RunMessageLoop();
};

