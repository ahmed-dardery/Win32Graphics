#include "Win32.h"

 COLORREF *(Win32::bmp) = 0;

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

int lastWidth, lastHeight;
HDC lastHDC;
BITMAPINFO lastBitInfo = {};
HBITMAP Membitmap = nullptr;
#include <stdio.h>

void Win32::StartFastPixel(HWND hwnd, HDC hdc)
{
    //Get window dimensions and allocate memory if necessary.
    RECT rect;
    GetClientRect(hwnd, &rect);
    int newWidth = rect.right - rect.left;
    int newHeight = rect.bottom - rect.top;
    if (newWidth != lastWidth || newHeight != lastHeight) {
        lastWidth = newWidth;
        lastHeight = newHeight;

        delete[] bmp;
        bmp = new COLORREF[(size_t)lastWidth * lastHeight];
    }

    //Create a new hdc
    HDC hCaptureDC = CreateCompatibleDC(hdc);
    //create a new bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, lastWidth, lastHeight);
    //link the newly created hdc with the newly created bitmap
    SelectObject(hCaptureDC, hBitmap);
    //copy the pixels from the original hdc to the new one
    BOOL bOK = BitBlt(hCaptureDC, 0, 0, lastWidth, lastHeight, hdc, 0, 0, SRCCOPY);
    //delete the hdc and bitmap
    DeleteDC(hCaptureDC);

    //Required options to get pixels in a proper format.
    BITMAPINFOHEADER& h = lastBitInfo.bmiHeader;
    h.biSize = sizeof(BITMAPINFOHEADER);
    h.biWidth = lastWidth;
    h.biHeight = -lastHeight;
    h.biPlanes = 1;
    h.biBitCount = 32;
    h.biCompression = BI_RGB;
    h.biSizeImage = lastWidth * lastHeight;

    //Get the bitmap buffer
    GetDIBits(hdc, hBitmap, 0, lastHeight, bmp, &lastBitInfo, DIB_RGB_COLORS);

    //Delete the bitmap
    DeleteObject(hBitmap);
}

void Win32::SetPixelFast(HDC hdc, int x, int y, COLORREF color)
{
    if (x < 0 || y < 0 || x >= lastWidth || y >= lastHeight) return;

    //for some reason, the format of the bitmap is BGR not RGB, so we need to swap
    //the R component with the B component.
    COLORREF c = RGB(GetBValue(color), GetGValue(color), GetRValue(color));

    //Set the pixel directly without API calls.
    bmp[y * lastWidth + x] = c;
}

void Win32::EndFastPixel(HDC hdc)
{
    //Copy the buffer into hdc.
    SetDIBitsToDevice(hdc, 0, 0, lastWidth, lastHeight, 0, 0, 0, lastHeight, bmp, &lastBitInfo, DIB_RGB_COLORS);
}

HDC hdc = nullptr, Orghdc = nullptr;
int win_width = 0, win_height = 0; 

//DoubleBuffer is not compatible with hbrBackground in WndClass.
//If you decide to use double buffer, you must disable it by setting
//hbrBackground to NULL during the initialization of the window.

HDC Win32::BeginDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps) {
    RECT Client_Rect;
    GetClientRect(hwnd, &Client_Rect);
    win_width = Client_Rect.right - Client_Rect.left;
    win_height = Client_Rect.bottom + Client_Rect.left;
    Orghdc = BeginPaint(hwnd, ps);
    hdc = CreateCompatibleDC(Orghdc); //create a copy of Orghdc
    Membitmap = CreateCompatibleBitmap(Orghdc, win_width, win_height); //create a bitmap that fits into hdc
    
    SelectObject(hdc, Membitmap); //links hdc with the bitmap
    BitBlt(hdc, 0, 0, win_width, win_height, Orghdc, 0, 0, SRCCOPY);
       
    return hdc;
}

void Win32::EndDoubleBufferPaint(HWND hwnd, PAINTSTRUCT* ps)
{
    BitBlt(Orghdc, 0, 0, win_width, win_height, hdc, 0, 0, SRCCOPY);
    DeleteObject(Membitmap);
    Membitmap = nullptr;
    DeleteDC(Orghdc);
    DeleteDC(hdc);
}
