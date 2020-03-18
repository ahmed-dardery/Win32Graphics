#include <windows.h>
#include <algorithm>
#include <windowsx.h>
#include "GraphicsAlgo.h"
#include "MenuHandler.h"
#include "Painter.h"

Painter &painter = Painter::getInstance();
bool isClicked;

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
    wc.hbrBackground = NULL;
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
        HDC hdc = nullptr, Orghdc = nullptr;
        PAINTSTRUCT ps;
        int win_width = 0, win_height = 0; 
        HBITMAP Membitmap = nullptr;

        if (doubleBuffer) {
            RECT Client_Rect;
            GetClientRect(hwnd, &Client_Rect);
            win_width = Client_Rect.right - Client_Rect.left;
            win_height = Client_Rect.bottom + Client_Rect.left;
            Orghdc = BeginPaint(hwnd, &ps);
            hdc = CreateCompatibleDC(Orghdc);
            Membitmap = CreateCompatibleBitmap(Orghdc, win_width, win_height);
            SelectObject(hdc, Membitmap);
        }
        else {
            hdc = BeginPaint(hwnd, &ps);
        }
      
        painter.ClearAll(ps, hdc);
        painter.PaintProcedure(hdc);

        if (doubleBuffer) {

            BitBlt(Orghdc, 0, 0, win_width, win_height, hdc, 0, 0, SRCCOPY);
            DeleteObject(Membitmap);
            DeleteDC(Orghdc);
        }

        DeleteDC(hdc);
        EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_MOUSEMOVE: {
        if (isClicked) {
            painter.announceDragged(LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hwnd, NULL, true);
        }
        return 0;
    }
    case WM_LBUTTONDOWN: {
        isClicked = 1;
        painter.announceClicked(LOWORD(lParam), HIWORD(lParam));

        InvalidateRect(hwnd, NULL, true);
        return 0;
    }
    case WM_LBUTTONUP: {
        isClicked = 0;
        return 0;
    }
    case WM_RBUTTONDOWN: {
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}