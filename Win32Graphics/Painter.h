#pragma once
#include <Windows.h>

class Painter
{
private:

	Painter();
	Painter(const Painter& rhs);
	static Painter instance;
public:
	enum class mode {
		DRAWLINE = 0,
		DRAGSTART = 1,
		DRAGEND = 2
	};
	static Painter& getInstance();
	int xst = 0, yst = 0, xen = 0, yen = 0;
	int xstleg = 0, ystleg = 0;
	int xenleg = 0, yenleg = 0;
	bool drawLegStart = 0, drawLegEnd = 0;
	bool lineSet = 0;

	bool clearScreen = 1;
	HDC lastHDC = NULL;

	COLORREF forecolor = RGB(0, 0, 0);
	COLORREF backcolor = RGB(255, 255, 255);

	mode current = mode::DRAWLINE;

	void ClearAll(PAINTSTRUCT& ps, HDC hdc);
	void PaintProcedure(HWND hwnd, HDC hdc);

	void announceClicked(int x, int y);
	void announceDragged(int x, int y);

	void queueClearScreen();
};