#include "GraphicsAlgo.h"
#include <cmath>
#include <algorithm>

void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color) {
    const double dtheta = 1.0 / R;
    const double sin_dtheta = sin(dtheta);
    const double cos_dtheta = cos(dtheta);
    const double M_PI = acos(-1.0);

    const auto SetMultiPixel = [hdc, xc, yc, color](int x, int y) {
        SetPixel(hdc, xc + x, yc + y, color);
        SetPixel(hdc, xc - x, yc + y, color);
        SetPixel(hdc, xc + x, yc - y, color);
        SetPixel(hdc, xc - x, yc - y, color);

        SetPixel(hdc, xc + y, yc + x, color);
        SetPixel(hdc, xc - y, yc + x, color);
        SetPixel(hdc, xc + y, yc - x, color);
        SetPixel(hdc, xc - y, yc - x, color);
    };

    double x = R;
    double y = 0;
    SetMultiPixel(x, y);

    while (x > y)
    {
        double xnew = x * cos_dtheta - y * sin_dtheta;
        double ynew = x * sin_dtheta + y * cos_dtheta;

        SetMultiPixel(ROUND(x), ROUND(y));

        x = xnew;
        y = ynew;
    }
}

void FillCircle(HDC hdc, int xc, int yc, int R, COLORREF color) {
    const double dtheta = 1.0 / R;
    const double sin_dtheta = sin(dtheta);
    const double cos_dtheta = cos(dtheta);
    const double M_PI = acos(-1.0);

    const auto SetMultiPixel = [hdc, xc, yc, color](int x, int y) {
        DrawLine(hdc, xc, yc, xc + x, yc + y, color);
        DrawLine(hdc, xc, yc, xc - x, yc + y, color);
        DrawLine(hdc, xc, yc, xc + x, yc - y, color);
        DrawLine(hdc, xc, yc, xc - x, yc - y, color);

        DrawLine(hdc, xc, yc, xc + y, yc + x, color);
        DrawLine(hdc, xc, yc, xc - y, yc + x, color);
        DrawLine(hdc, xc, yc, xc + y, yc - x, color);
        DrawLine(hdc, xc, yc, xc - y, yc - x, color);
    };

    double x = R;
    double y = 0;
    SetMultiPixel(x, y);

    while (x > y)
    {
        double xnew = x * cos_dtheta - y * sin_dtheta;
        double ynew = x * sin_dtheta + y * cos_dtheta;

        SetMultiPixel(ROUND(x), ROUND(y));

        x = xnew;
        y = ynew;
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