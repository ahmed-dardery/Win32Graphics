#include <windows.h>
#include <algorithm>
#include "GraphicsAlgo.h"
#include "MenuHandler.h"
#include "Painter.h"
#include "Win32.h"

Painter &painter = Painter::getInstance();

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main(int argc, char* argv[]){
    return WinMain(GetModuleHandle(NULL), NULL, argv[0], 1);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {

    HWND hwnd = Win32::CreateFunctionalWindow(hInstance, WndProc, 
        RECT{ CW_USEDEFAULT ,CW_USEDEFAULT ,800,600 }, 
        L"Win32 Graphics", MenuHandler::getMenuName());

    MenuHandler::initializeMenu(hwnd);

    Win32::RunMessageLoop();

    return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    case WM_COMMAND: {
        MenuHandler::Action ac = MenuHandler::performMenuAction(hwnd, wParam);
        if (ac == MenuHandler::Action::CLEAR_SCREEN) {
            painter.lineSet = 0;
            InvalidateRect(hwnd, NULL, true);
        }
        return 0;
    }
    case WM_PAINT: {
        bool doubleBuffer = MenuHandler::Menu().DoubleBuffer.isChecked();
        HDC hdc = nullptr;
        PAINTSTRUCT ps;

        if (doubleBuffer)
            hdc = Win32::BeginDoubleBufferPaint(hwnd, &ps);
        else 
            hdc = BeginPaint(hwnd, &ps);
      
        
        if (painter.clearScreen) {
            painter.ClearAll(ps, hdc);
            painter.clearScreen = 0;
        }
        else {
            painter.ClearAll(ps, hdc);
            painter.PaintProcedure(hwnd, hdc);
        }

        if (doubleBuffer)
            Win32::EndDoubleBufferPaint(hwnd, &ps);
        else
            EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_MOUSEMOVE: {
        if (wParam & MK_LBUTTON) {
            painter.announceDragged(LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hwnd, NULL, true);
        }
        return 0;
    }
    case WM_LBUTTONDOWN: {
        painter.announceClicked(LOWORD(lParam), HIWORD(lParam));

        InvalidateRect(hwnd, NULL, true);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}