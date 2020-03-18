#include "Painter.h"
#include "MenuHandler.h"
#include "GraphicsAlgo.h"
Painter::Painter() {
}
Painter::Painter(const Painter& rhs) {
}
void Painter::ClearAll(PAINTSTRUCT& ps, HDC hdc) {
    FillRect(hdc, &ps.rcPaint, CreateSolidBrush(backcolor));
}
void Painter::PaintProcedure(HDC hdc) {
    if (lineSet) {
        if (MenuHandler::Menu().DrawLine.getCheckedIndex()==0) 
            DrawLine(hdc, xst, yst, xen, yen, forecolor); 
        else{
            DrawBezier(hdc, xst, yst, 4*(xst-xstleg), 4*(yst-ystleg), xen, yen, -4*(xen-xenleg), -4*(yen-yenleg), forecolor);
            DrawCircle(hdc, xst, yst, 7, forecolor);
            DrawCircle(hdc, xen, yen, 7, forecolor);

            DrawLine(hdc, xst, yst, xstleg, ystleg, forecolor);
            DrawLine(hdc, xen, yen, xenleg, yenleg, forecolor);
        }
        int circleType = MenuHandler::Menu().DrawCircle.getCheckedIndex();
        if (circleType == 1) {
            int r = (xst - xen) * (xst - xen) + (yst - yen) * (yst - yen);
            DrawCircle(hdc, xst, yst, ROUND(sqrt(r)), forecolor);
        }
        else if (circleType == 2) {
            int r = (xst - xen) * (xst - xen) + (yst - yen) * (yst - yen);
            FillCircle(hdc, xst, yst, ROUND(sqrt(r)), forecolor);
        }

    }
}

void Painter::announceClicked(int x, int y)
{
    int selectedPoint = 0;
    if (lineSet) {
        if (abs(x - xst) < 8 && abs(y - yst) < 8)
            selectedPoint = 1;
        else if (abs(x - xen) < 8 && abs(y - yen) < 8)
            selectedPoint = 2;
    }
    current = (mode)selectedPoint;

    if (selectedPoint == 0) {
        xstleg= xst = x;
        ystleg= yst = y;
        lineSet = 0;
    }


}

void Painter::announceDragged(int x, int y) {
    if (current == mode::DRAGSTART) {
        xstleg = x;
        ystleg = y;
    }
    else if (current == mode::DRAGEND) {
        xenleg = x;
        yenleg = y;
    }
    else {
        xenleg = xen = x;
        yenleg = yen = y;
        lineSet = 1;
    }

}

Painter& Painter::getInstance() {
    return instance;
}
Painter Painter::instance;