#include <windows.h>
#include <algorithm>
#include <windowsx.h>
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

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Class Name";

    WNDCLASS wc = {};   //initialize everything to null

    wc.lpfnWndProc = WndProc;     //pointer to windows procedure function
    wc.hInstance = hInstance;    //handle to instance
    wc.lpszClassName = CLASS_NAME;  //pointer to string class name
    wc.hbrBackground = NULL; //CreateSolidBrush(RGB(255, 255, 255));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = MenuHandler::getMenuName();
    if (!RegisterClass(&wc))
        return 0;
    
    // Create the window.

    //for options check
    //https://docs.microsoft.com/en-us/previous-versions/ms960010(v=msdn.10)
    HWND hwnd = CreateWindowEx(
        0,                           // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Garbage Drawer",             // Window title
        WS_OVERLAPPEDWINDOW,            // Window style

        //Location (x, y) Size (width, height)
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    //if creation fails for any reason
    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    MenuHandler::initializeMenu(hwnd);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

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