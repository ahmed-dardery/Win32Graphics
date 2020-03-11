#pragma once
#include <Windows.h>
class Win32
{
public:
	static bool showColorDialog(HWND hwnd, COLORREF& choice);
};

