#include <Windows.h>
#include "MenuHandler.h"
#include "resource.h"
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

    int colorsIDs[] = { ID_COLORS_RED, ID_COLORS_GREEN, ID_COLORS_BLUE };
    MainMenu::MainMenu(HMENU menu) :
        DrawLine(menu, ID_OPTIONS_DRAWLINE),
        DrawCircle(menu, ID_OPTIONS_DRAWCIRCLE),
        Colors(menu, colorsIDs, sizeof colorsIDs / sizeof colorsIDs[0]),
        TrailDraw(menu, ID_OPTIONS_TRAILDRAW)
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
    void performMenuAction(HWND hwnd, LPARAM wParam) {
        switch (LOWORD(wParam))
        {
            case ID_OPTIONS_DRAWLINE: {
                menu.DrawLine.toggleChecked();
                return;
            }
            case ID_OPTIONS_DRAWCIRCLE: {
                menu.DrawCircle.toggleChecked();
                return;
            }
            case ID_OPTIONS_TRAILDRAW: {
                menu.TrailDraw.toggleChecked();
                return;
            }
            case ID_OPTIONS_CLEARSCREEN: {
                InvalidateRect(hwnd, NULL, true);
                return;
            }
            case ID_COLORS_RED:
            case ID_COLORS_BLUE:
            case ID_COLORS_GREEN:
            {
                menu.Colors.setChecked(LOWORD(wParam));
                return;
            }
        }


    }
}