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
        if (MenuHandler::Menu().DrawLine.isChecked()) {
            if (drawLegEnd && drawLegStart)
                drawCubic(hdc, xst, yst, 3*(xst-xstleg), 3*(yst-ystleg), xen, yen, -3*(xen-xenleg), -3*(yen-yenleg), forecolor);
            else
                DrawLine(hdc, xst, yst, xen, yen, forecolor);

            if (drawLegStart) DrawLine(hdc, xst, yst, xstleg, ystleg, forecolor);
            if (drawLegEnd) DrawLine(hdc, xen, yen, xenleg, yenleg, forecolor);
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
        xst = x;
        yst = y;
        drawLegStart = drawLegEnd = lineSet = 0;
    }


}

void Painter::announceDragged(int x, int y) {
    if (current == mode::DRAGSTART) {
        drawLegStart = 1;
        xstleg = x;
        ystleg = y;
    }
    else if (current == mode::DRAGEND) {
        drawLegEnd = 1;
        xenleg = x;
        yenleg = y;
    }
    else {
        xen = x;
        yen = y;
        lineSet = 1;
    }

}

Painter& Painter::getInstance() {
    return instance;
}
Painter Painter::instance;