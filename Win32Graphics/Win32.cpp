#include "Win32.h"

bool Win32::showColorDialog(HWND hwnd, COLORREF& ret) {
    CHOOSECOLOR cc = {};                 // common dialog box structure 
    static COLORREF acrCustClr[16]; // array of custom colors 
    // Initialize CHOOSECOLOR 
    
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = ret;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    
    if (!ChooseColor(&cc)) return false;
    ret = cc.rgbResult;
   
    return true;
}

#pragma region FASTPIXEL

COLORREF* bmp = 0;
bool fastpixelmode = 0;
BITMAPINFO bmpInfo = { {sizeof(BITMAPINFOHEADER), 0, 0, 1, 32, BI_RGB} };
BITMAPINFOHEADER& bmpHeader = bmpInfo.bmiHeader;
void Win32::StartFastPixel(HDC hdc)
{
    //Get window dimensions and allocate memory if necessary.
    int newWidth, newHeight;
    getScreenResolution(hdc, newWidth, newHeight);

    if (newWidth != bmpHeader.biWidth || newHeight != -bmpHeader.biHeight) {
        //screen resized, we need a new buffer size.
        
        bmpHeader.biWidth = newWidth;
        bmpHeader.biHeight = -newHeight;
        bmpHeader.biSizeImage = newWidth * newHeight;

        delete[] bmp;
        bmp = new COLORREF[(size_t)newWidth * newHeight];
    }

    //the HBITMAP that the inital hdc uses contains the border as well, so we need to clone it without border
    HDC hdcClone = cloneHDC(hdc);
    HBITMAP hBitmap = (HBITMAP)GetCurrentObject(hdcClone, OBJ_BITMAP);
    
    //Get the bitmap buffer
    GetDIBits(hdcClone, hBitmap, 0, -bmpHeader.biHeight, bmp, &bmpInfo, DIB_RGB_COLORS);
    
    //Delete the bitmap
    DeleteObject(hBitmap);
    DeleteDC(hdcClone);
    fastpixelmode = 1;
}

void Win32::SetPixelFast(HDC hdc, int x, int y, COLORREF color)
{
    if (!fastpixelmode) {
        SetPixel(hdc, x, y, color);
        return;
    }
    if (x < 0 || y < 0 || x >= bmpHeader.biWidth || y >= -bmpHeader.biHeight) return;

    //for some reason, the format of the bitmap is BGR not RGB, so we need to swap
    //the R component with the B component.
    COLORREF c = RGB(GetBValue(color), GetGValue(color), GetRValue(color));

    //Set the pixel directly without API calls.
    bmp[y * bmpHeader.biWidth + x] = c;
}

COLORREF Win32::GetPixelFast(HDC hdc, int x, int y)
{
    
    if (x < 0 || y < 0 || x >= bmpHeader.biWidth || y >= -bmpHeader.biHeight) return 0xFFFFFFFF;

    COLORREF color = bmp[y * bmpHeader.biWidth + x];
    return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
}

void Win32::EndFastPixel(HDC hdc)
{
    fastpixelmode = 0;
    //Copy the buffer into hdc.
    SetDIBitsToDevice(hdc, 0, 0, bmpHeader.biWidth, -bmpHeader.biHeight, 0, 0, 0, -bmpHeader.biHeight, bmp, &bmpInfo, DIB_RGB_COLORS);
}

#pragma endregion

#pragma region DoubleBuffer

HDC hdc = nullptr, Orghdc = nullptr;
int win_width = 0, win_height = 0; 

HDC Win32::BeginDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps) {
    Orghdc = BeginPaint(hwnd, ps);
    getScreenResolution(Orghdc, win_width, win_height);
    hdc = cloneHDC(Orghdc);
    return hdc;
}

void Win32::EndDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps)
{
    BitBlt(Orghdc, 0, 0, win_width, win_height, hdc, 0, 0, SRCCOPY);
    DeleteDC(hdc);
    EndPaint(hwnd, ps);
    hdc = Orghdc = NULL;
    win_width = win_height = 0;
}

#pragma endregion

HDC Win32::cloneHDC(HDC Orghdc) {
    int width, height;
    getScreenResolution(Orghdc, width, height);

    HDC hdc = CreateCompatibleDC(Orghdc); //create a copy of Orghdc
    HBITMAP Membitmap = CreateCompatibleBitmap(Orghdc, width, height); //create a bitmap that fits into hdc

    SelectObject(hdc, Membitmap); //links hdc with the bitmap
    DeleteObject(Membitmap);

    BitBlt(hdc, 0, 0, width, height, Orghdc, 0, 0, SRCCOPY);

    return hdc;
}

void Win32::getScreenResolution(HDC hdc, int& width, int& height)
{
    RECT rect;
    GetClipBox(hdc, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}


HWND Win32::CreateFunctionalWindow(HINSTANCE hInstance, WNDPROC lpfnWndProc, RECT window, LPCWSTR windowTitle, LPCWSTR menu) {
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Class Name";

    WNDCLASS wc = {};   //initialize everything to null

    wc.lpfnWndProc = lpfnWndProc;     //pointer to windows procedure function
    wc.hInstance = hInstance;    //handle to instance
    wc.lpszClassName = CLASS_NAME;  //pointer to string class name
    wc.hbrBackground = NULL; //CreateSolidBrush(RGB(255, 255, 255));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = menu;
    if (!RegisterClass(&wc))
        return 0;

    // Create the window.

    //for options check
    //https://docs.microsoft.com/en-us/previous-versions/ms960010(v=msdn.10)
    HWND hwnd = CreateWindowEx(
        0,                           // Optional window styles.
        CLASS_NAME,                     // Window class
        windowTitle,             // Window title
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

    ShowWindow(hwnd, 1);

    return hwnd;
}

void Win32::RunMessageLoop()
{
    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
