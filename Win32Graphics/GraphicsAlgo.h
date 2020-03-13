#pragma once
#include <Windows.h>
#include <math.h>
#define ROUND(v) int(v + (v>0?0.5:-0.5))

void DrawLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color);
void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color);
void FillCircle(HDC hdc, int xc, int yc, int R, COLORREF color);

void drawCubic(HDC hdc, int x1, int y1, int u1, int v1, int x2, int y2, int u2, int v2, COLORREF color);