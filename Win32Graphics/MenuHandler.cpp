#include <Windows.h>
#include "MenuHandler.h"
#include "resource.h"
#include "Win32.h"
#include "Painter.h"
namespace MenuHandler {
    MainMenu menu;

    CheckMenu::CheckMenu() {

    }
    CheckMenu::CheckMenu(HMENU hlp, int ID) : hlp(hlp), ID(ID) {
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_STATE;
        GetMenuItemInfo(hlp, ID, NULL, &mii);
    }
    bool CheckMenu::isChecked() {
        return mii.fState == MFS_CHECKED;
    }
    void CheckMenu::setChecked(bool v) {
        mii.fState = v ? MFS_CHECKED : MFS_UNCHECKED;
        SetMenuItemInfo(hlp, ID, NULL, &mii);
    }
    void CheckMenu::toggleChecked() {
        mii.fState ^= MFS_CHECKED;
        SetMenuItemInfo(hlp, ID, NULL, &mii);
    }

    int CheckMenu::getID()
    {
        return ID;
    }

    GroupMenu::GroupMenu()
    {
    }
    GroupMenu::GroupMenu(HMENU hlp, int* ids, int n)
    {
        nCnt = n;
        choices = new CheckMenu[n];
        for (int i = 0; i < n; i++)
            choices[i] = CheckMenu(hlp, ids[i]);
    }
    int GroupMenu::getCheckedIndex()
    {
        for (int i = 0; i < nCnt; i++)
            if (choices[i].isChecked()) return i;
        return -1;
    }
    void GroupMenu::setChecked(int id)
    {
        for (int i = 0; i < nCnt; i++) {
            choices[i].setChecked(choices[i].getID() == id);
        }
    }

    MainMenu::MainMenu() {

    }

    int circleIDs[] = { ID_DRAWINGS_NOCIRCLE, ID_DRAWINGS_DRAWCIRCLE, ID_DRAWINGS_FILLCIRCLE };
    MainMenu::MainMenu(HMENU menu) :
        DrawLine(menu, ID_DRAWINGS_DRAWLINE),
        DrawCircle(menu, circleIDs, sizeof circleIDs / sizeof circleIDs[0]),
        DoubleBuffer(menu, ID_OPTIONS_DOUBLEBUFFER)
    {    }

    LPCWSTR getMenuName() {
        return MAKEINTRESOURCE(IDR_MENU1);
    }

    void initializeMenu(HWND hwnd) {
        menu = MainMenu(GetMenu(hwnd));
    }
    MainMenu Menu()
    {
        return menu;
    }
    Action performMenuAction(HWND hwnd, LPARAM wParam) {
        switch (LOWORD(wParam))
        {
            case ID_DRAWINGS_DRAWLINE: {
                menu.DrawLine.toggleChecked();
                return Action::NO_ACTION;
            }
            case ID_DRAWINGS_NOCIRCLE:
            case ID_DRAWINGS_DRAWCIRCLE:
            case ID_DRAWINGS_FILLCIRCLE: {
                menu.DrawCircle.setChecked(LOWORD(wParam));
                return Action::NO_ACTION;
            }
            case ID_OPTIONS_DOUBLEBUFFER: {
                menu.DoubleBuffer.toggleChecked();
                return Action::NO_ACTION;
            }
            case ID_OPTIONS_CLEARSCREEN: {
                InvalidateRect(hwnd, NULL, true);
                return Action::CLEAR_SCREEN;
            }
            case ID_COLORS_FOREGROUND: {
                Win32::showColorDialog(hwnd, Painter::getInstance().forecolor);
                InvalidateRect(hwnd, NULL, true);
                return Action::NO_ACTION;
            }
            case ID_COLORS_BACKGROUND: {
                Win32::showColorDialog(hwnd, Painter::getInstance().backcolor);
                InvalidateRect(hwnd, NULL, true);
                return Action::NO_ACTION;
            }
            default: {
                return Action::NO_ACTION;
            }
        }


    }
}