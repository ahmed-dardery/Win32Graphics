#include "GraphicsAlgo.h"
#include <cmath>
#include <algorithm>
#include "Win32.h"

#define SetPixel Win32::SetPixelFast

void SetMultiPixel(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);

    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc - x, color);
}

void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color) {
    int x = R;
    int y = 0;
    SetMultiPixel(hdc,xc,yc,color,x, y);
    int d = 5 - 4 * R;
    while (x > y)
    {
        if (d > 0) {
            d += -8 * x + 8;
            x--;
        }
        d += 8 * y + 12;
        y++;

        SetMultiPixel(hdc, xc, yc, color, x,y);
    }
}

void DrawCircleDeprecated(HDC hdc, int xc, int yc, int R, COLORREF color) {
    const double dtheta = 1.0 / R;
    const double sin_dtheta = sin(dtheta);
    const double cos_dtheta = cos(dtheta);

    double x = R;
    double y = 0;
    SetMultiPixel(hdc, xc, yc, color, x, y);

    while (x > y)
    {
        double xnew = x * cos_dtheta - y * sin_dtheta;
        double ynew = x * sin_dtheta + y * cos_dtheta;

        SetMultiPixel(hdc, xc, yc, color, ROUND(xnew), ROUND(ynew));

        x = xnew;
        y = ynew;
    }
}

void FillCircle(HDC hdc, int xc, int yc, int R, COLORREF color) {
    for (int x = -R; x < +R; x++)
    {
        for (int y = -R; y < +R; y++)
        {
            if (x * x + y * y < R * R)
            SetPixel(hdc, x+xc, y+yc, color);
        }
    }
}

void DrawLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;
    if (abs(dx) >= abs(dy)) {
        if (xe < xs) std::swap(xs, xe), std::swap(ys, ye);
        SetPixel(hdc, xs, ys, color);

        int x = xs;
        int y = ys;
        int diff = ye > ys ? 1 : -1;
        int d = 2 * dy - diff * dx;

        int dir = -dx * diff;
        while (x < xe) {
            ++x;
            if (d * dir < 0) {
                y += diff, d -= 2 * dx * diff;
                //SetPixel(hdc, x-1, y, color);
            }
            d += 2 * dy;
            SetPixel(hdc, x, y, color);
        }
    }
    else {
        if (ye < ys) std::swap(xs, xe), std::swap(ys, ye);
        SetPixel(hdc, xs, ys, color);

        int x = xs;
        int y = ys;
        int diff = xe > xs ? 1 : -1;
        int d = dy * diff - 2 * dx;

        int dir = dy * diff;

        while (y < ye) {
            if (d * dir < 0) {
                x += diff, d += 2 * dy * diff;
                //SetPixel(hdc, x, y-1, color);
            }
            ++y;
            d -= 2 * dx;
            SetPixel(hdc, x, y, color);
        }
    }
}

void DrawLineDouble(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;
    if (abs(dx) >= abs(dy)) {
        if (xe < xs) std::swap(xs, xe), std::swap(ys, ye);
        SetPixel(hdc, xs, ys, color);
        int x = xs;
        double y = ys;
        double m = (double)dy / dx;
        while (x < xe) {
            ++x;
            y += m;
            SetPixel(hdc, x, ROUND(y), color);
        }
    }
    else {
        if (ye < ys) std::swap(xs, xe), std::swap(ys, ye);
        SetPixel(hdc, xs, ys, color);
        double x = xs;
        int y = ys;
        double m_inv = (double)dx / dy;
        while (y < ye) {
            ++y;
            x += m_inv;
            SetPixel(hdc, ROUND(x), y, color);
        }
    }
}
/*
void DrawBezier(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLORREF color) {
    x2 = 3 * (x2 - x1);
    y2 = 3 * (y2 - y1);
    x3 = 3 * (x4 - x3);
    y3 = 3 * (y4 - y3);
    int a0 = x1;
    int a1 = x2;
    int a2 = -3 * x1 - 2 * x2 + 3 * x4 - x3;
    int a3 = 2 * x1 + x2 - 2 * x4 + x3;

    int b0 = y1;
    int b1 = y2;
    int b2 = -3 * y1 - 2 * y2 + 3 * y4 - y3;
    int b3 = 2 * y1 + y2 - 2 * y4 + y3;

    double dt = 0.0005;
    for (double t = 0; t <= 1; t += dt)
    {
        int x = a0 + a1 * t + a2 * t * t + a3 * t * t * t;
        int y = b0 + b1 * t + b2 * t * t + b3 * t * t * t;
       
        SetPixel(hdc, ROUND(x), ROUND(y), color);
    }
}*/

void DrawBezier(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLORREF color) {
    
    int a3 = -x1 + 3 * x2 - 3 * x3 + x4;
    int a2 = 3 * x1 - 6 * x2 + 3 * x3;
    int a1 = -3 * x1 + 3 * x2;
    int a0 = x1;

    int b3 = -y1 + 3 * y2 - 3 * y3 + y4;
    int b2 = 3 * y1 - 6 * y2 + 3 * y3;
    int b1 = -3 * y1 + 3 * y2;
    int b0 = y1;

    double dt = 0.0005;
    for (double t = 0; t <= 1; t += dt)
    {
        int x = a0 + a1 * t + a2 * t * t + a3 * t * t * t;
        int y = b0 + b1 * t + b2 * t * t + b3 * t * t * t;

        SetPixel(hdc, ROUND(x), ROUND(y), color);
    }
}