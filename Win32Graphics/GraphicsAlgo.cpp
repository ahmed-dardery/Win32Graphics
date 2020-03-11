#include "GraphicsAlgo.h"
#include <cmath>
#include <algorithm>

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
    //TO BE IMPLEMENTED
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
            if (d * dir < 0) y += diff, d -= 2 * dx * diff;
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
            if (d * dir < 0) x += diff, d += 2 * dy * diff;
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