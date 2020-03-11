#pragma once
#include <Windows.h>
namespace MenuHandler {
    enum class Action {
        CLEAR_SCREEN,
        NO_ACTION
    };

    LPCWSTR getMenuName();
    Action performMenuAction(HWND hwnd, LPARAM);
    void initializeMenu(HWND);

    class CheckMenu {
    private:
        MENUITEMINFO mii = {};
        HMENU hlp;
        int ID;
    public:
        CheckMenu();
        CheckMenu(HMENU hlp, int ID);
        bool isChecked();
        void setChecked(bool v);
        void toggleChecked();
        int getID();
    };

    class GroupMenu {
    private:
        CheckMenu* choices;
        int nCnt;
    public:
        GroupMenu();
        GroupMenu(HMENU hlp, int* ids, int n);
        int getCheckedIndex();
        void setChecked(int id);
    };

    class MainMenu {
    public:
        CheckMenu DrawLine;
        GroupMenu DrawCircle;
        CheckMenu DoubleBuffer;
        MainMenu();
        MainMenu(HMENU menu);
    };

    MainMenu Menu();
}