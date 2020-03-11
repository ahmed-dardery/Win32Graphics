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
        if (MenuHandler::Menu().DrawLine.isChecked())
            DrawLine(hdc, xst, yst, xen, yen, forecolor);

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

Painter& Painter::getInstance() {
    return instance;
}
Painter Painter::instance;