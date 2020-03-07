#include <windows.h>
#include <algorithm>
#include <windowsx.h>
#include "GraphicsAlgo.h"
#include "MenuHandler.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main() {
    return WinMain(GetModuleHandle(NULL), NULL, NULL, 1);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Class Name";

    WNDCLASS wc = {};   //initialize everything to null

    wc.lpfnWndProc = WndProc;     //pointer to windows procedure function
    wc.hInstance = hInstance;    //handle to instance
    wc.lpszClassName = CLASS_NAME;  //pointer to string class name
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
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

int xst, yst, xen, yen;
bool lineSet = 0;
bool isClicked = 0;
int coloridx;

COLORREF colors[] = { RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),RGB(255,255,0),RGB(255,0,255),RGB(0,255,255),RGB(0,0,0) };
const int colorn = sizeof colors / sizeof colors[0];
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    case WM_COMMAND: {
        MenuHandler::performMenuAction(hwnd, wParam);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (lineSet) {
            coloridx = MenuHandler::Menu().Colors.getCheckedIndex();

            if (MenuHandler::Menu().DrawLine.isChecked())
                DrawLine(hdc, xst, yst, xen, yen, colors[coloridx]);

            int circleType = MenuHandler::Menu().DrawCircle.getCheckedIndex();
            if (circleType == 1) {
                int r = (xst - xen) * (xst - xen) + (yst - yen) * (yst - yen);
                DrawCircle(hdc, xst, yst, ROUND(sqrt(r)), colors[coloridx]);
            }else if (circleType == 2){
                int r = (xst - xen) * (xst - xen) + (yst - yen) * (yst - yen);
                FillCircle(hdc, xst, yst, ROUND(sqrt(r)), colors[coloridx]);
            }
                
        }    

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_MOUSEMOVE: {
        if (isClicked) {
            xen = LOWORD(lParam);
            yen = HIWORD(lParam);
            lineSet = 1;
            InvalidateRect(hwnd, NULL, !MenuHandler::Menu().TrailDraw.isChecked());
        }
        return 0;
    }
    case WM_LBUTTONDOWN: {
        isClicked = 1;
        xen = xst = LOWORD(lParam);
        yen = yst = HIWORD(lParam);
        lineSet = 1;
        InvalidateRect(hwnd, NULL, !MenuHandler::Menu().TrailDraw.isChecked());
        return 0;
    }
    case WM_LBUTTONUP: {
        isClicked = 0;
        return 0;
    }
    case WM_RBUTTONDOWN: {
        coloridx = (coloridx + 1) % colorn;
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}