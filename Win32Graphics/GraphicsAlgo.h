#pragma once
#include <Windows.h>

#define ROUND(v) int(v + 0.5)

void DrawLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color);
void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color);
