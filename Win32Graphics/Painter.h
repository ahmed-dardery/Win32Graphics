#pragma once
#include <Windows.h>

class Painter
{
private:
	Painter();
	Painter(const Painter& rhs);
	static Painter instance;
public:
	static Painter& getInstance();
	int xst = 0, yst = 0, xen = 0, yen = 0;
	bool lineSet = 0;

	COLORREF forecolor = RGB(0, 0, 0);
	COLORREF backcolor = RGB(255, 255, 255);

	void ClearAll(PAINTSTRUCT& ps, HDC hdc);
	void PaintProcedure(HDC hdc);

};