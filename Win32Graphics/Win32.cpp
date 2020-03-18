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
