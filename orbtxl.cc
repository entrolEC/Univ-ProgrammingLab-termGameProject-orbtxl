#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <queue>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "draw.h"
#include "space.h"
#include "shockwave.h"
#include "planet.h"

#define WIDTH 900
#define HEIGHT 900
#define SPACE 32
#define ORBIT 4
#define MAX_PLANET 20
#define RADIAN 57.2957951

HDC hdc, MemDC, tmpDC; 
PAINTSTRUCT ps;
HBITMAP BackBit, oldBackBit;
RECT bufferRT;
HWND hWnd;

int fps=1;
int cnt = 0;
int done = 0;


void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void update(Space* space) {

	startBuffer();

	drawOrbits(space);
	progressSpace(space);

	endBuffer();
}

int main() {
	int Delay = 10;
	unsigned char ch;
	clock_t CurTime, OldTime;
	
	int orbitArr[ORBIT] = { 400, 300, 200, 120 };
	Space* space = initSpace(orbitArr);
	srand(time(NULL));
	cls(0, 0);
	Sleep(10);  
	//PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_SYSTEM);

	while (1) {
		OldTime = clock();
		update(space);
		while (1)
		{
			CurTime = clock();
			if (CurTime - OldTime > 5)
				break;
		}
		fps++;
	}
	getch();

	return 0;
}