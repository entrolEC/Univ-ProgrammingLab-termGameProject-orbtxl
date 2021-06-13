#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

extern HDC hdc, MemDC, tmpDC;
extern PAINTSTRUCT ps;
extern HBITMAP BackBit, oldBackBit;
extern RECT bufferRT;
extern HWND hWnd;

extern int WIDTH, HEIGHT;

int* createRGB(int r, int g, int b) {
	int* rgb = (int*)malloc(sizeof(int) * 3);
	rgb[0] = r; rgb[1] = g; rgb[2] = b;
	return rgb;
}

void drawCircle(int x1, int y1, int x2, int y2, int* rgb, bool clearCircle) {
	HBRUSH Brush, oBrush;
	HPEN Pen, oPen;
	Pen = (HPEN)CreatePen(PS_SOLID, 2, RGB(rgb[0], rgb[1], rgb[2]));
	Brush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
	if (clearCircle)
		oBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	else
		oBrush = (HBRUSH)SelectObject(hdc, Brush);
	oPen = (HPEN)SelectObject(hdc, Pen);
	Ellipse(hdc, x1, y1, x2, y2);
	DeleteObject(Brush);
	DeleteObject(Pen);
	//ReleaseDC(hWnd, hdc);
}


void cls(int text_color, int bg_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "mode con:cols=110 lines=58 | COLOR %x%x", bg_color, text_color);
	system(cmd);

}

double calculateDistance(int x1, int x2, int y1, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void drawScore(double score) {
	int intScore = score;
	TCHAR ipOut[1024];
	int num = 1;
	wsprintf(ipOut, TEXT("%d"), intScore);
	TextOut(hdc, WIDTH / 2, HEIGHT / 2, ipOut, lstrlen(ipOut));
}


void startBuffer() {
	hWnd = GetForegroundWindow();
	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &bufferRT);
	MemDC = CreateCompatibleDC(hdc);
	BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
	oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
	PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
	tmpDC = hdc;
	hdc = MemDC;
	MemDC = tmpDC;
}

void endBuffer() {
	tmpDC = hdc;
	hdc = MemDC;
	MemDC = tmpDC;
	GetClientRect(hWnd, &bufferRT);
	BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBackBit);
	DeleteObject(BackBit);
	DeleteDC(MemDC);
	ReleaseDC(hWnd, hdc);
	EndPaint(hWnd, &ps);
}