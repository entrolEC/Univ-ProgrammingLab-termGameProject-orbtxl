#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <queue>

#define WIDTH 900
#define HEIGHT 900
#define SPACE 32
#define ORBIT 4
#define MAX_PLANET 15
#define RADIAN 57.2957951

static HDC hdc, MemDC, tmpDC; 
static PAINTSTRUCT ps;
static HBITMAP BackBit, oldBackBit;
static RECT bufferRT;
static HWND hWnd;
int fps=1;
int cnt = 0;

struct _Space;

typedef struct _Shockwave {
	int x;
	int y;
	double size;
	bool destructive;
	bool active;
	int* rgb;
} Shockwave;

typedef struct _Shadow {
	int x;
	int y;
	double size;
	bool destructive;
	bool active;
	int* rgb;
} Shadow;


typedef struct _Planet {
	int x;
	int y;
	double size;
	double velocity;
	double radius;
	double angle;
	bool active;
	int direction;
	int type;
	int timer;
	int centerX;
	int centerY;
	void (*blackhollEffect)(_Planet* planet, _Space * space);
	void (*collisionEffect)(_Planet* planet, _Space* space);
	void (*dyingEffect)(_Planet* planet, _Space* space);
	int* rgb;
} Planet;

typedef struct _BlackHoll {
	int x;
	int y;
	double size;
	bool active;
	int timer;
	void (*dyingEffect)(_BlackHoll* blackHoll);
	int* rgb;
} BlackHoll;

typedef struct _Space {
	int* orbit;
	Planet* earth;
	BlackHoll* mainBlackHoll;
	Planet** planets;
	BlackHoll** blackHolls;
	Shockwave** shockwaves;
	int currentOrbit;
	double score;
}Space;

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

int* createRGB(int r, int g, int b) {
	int* rgb = (int*)malloc(sizeof(int) * 3);
	rgb[0] = r; rgb[1] = g; rgb[2] = b;
	return rgb;
}

void drawCircle(int x1, int y1, int x2, int y2, int *rgb, bool clearCircle) {
	HBRUSH Brush, oBrush;
	HPEN Pen, oPen;
	Pen = (HPEN)CreatePen(PS_SOLID, 2, RGB(rgb[0], rgb[1], rgb[2]));
   	Brush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
	if(clearCircle)
		oBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	else
		oBrush = (HBRUSH)SelectObject(hdc, Brush);
	oPen = (HPEN)SelectObject(hdc, Pen);
	Ellipse(hdc, x1, y1, x2, y2);
	DeleteObject(Brush);
	DeleteObject(Pen);
	//ReleaseDC(hWnd, hdc);
}

Shockwave* initShockwave(int x, int y, double size, bool destructive, bool active, int* rgb) {
	Shockwave* shockwave = (Shockwave*)malloc(sizeof(Shockwave));
	shockwave->x = x;
	shockwave->y = y;
	shockwave->size = size;
	shockwave->destructive = destructive;
	shockwave->active = active;
	shockwave->rgb = rgb;
	return shockwave;
}

BlackHoll* initBlackHoll(int x, int y, double size, bool active, int timer, void dyingEffect(BlackHoll* blackHoll)) {
	BlackHoll* blackHoll = (BlackHoll*)malloc(sizeof(BlackHoll));
	blackHoll->x = x;
	blackHoll->y = y;
	blackHoll->size = size;
	blackHoll->active = active;
	blackHoll->timer = timer;
	blackHoll->dyingEffect = dyingEffect;
	blackHoll->rgb = createRGB(0, 0, 0);

	return blackHoll;
}

void smallerEffectBlackHoll(BlackHoll* blackHoll) {
	blackHoll->size *= 0.99;
	if (blackHoll->size <= 3) blackHoll->active = false;
}
 

void dyingEffect5(Planet* planet, Space* space) {
	planet->active = false;
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->blackHolls[i]->active) {
			space->blackHolls[i] = initBlackHoll(planet->x, planet->y, 25.0, 1, 3000,smallerEffectBlackHoll);
			break;
		}
	}
}

void dyingEffect4(Planet* planet, Space* space) {
	planet->active = false;
	space->earth->direction *= -1;
}

void dyingEffectNone(Planet* planet, Space* space) {
	planet->active = false;
}

void drawShockwave(Shockwave* shockwave) {
	drawCircle(shockwave->x - shockwave->size, shockwave->y - shockwave->size, shockwave->x + shockwave->size, shockwave->y + shockwave->size, shockwave->rgb, 1);
}

void progressShockwave(Shockwave** shockwaves) {
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!shockwaves[i]->active) continue;
		if (shockwaves[i]->size > 1000) shockwaves[i]->active = false;
		shockwaves[i]->size *= 1.01;
		drawShockwave(shockwaves[i]);
	}
}



void smallerEffect(Planet* planet, Space* space) {
	planet->size *= 0.98;
	planet->velocity *= 0.99;
	if (planet->size <= 5) planet->dyingEffect(planet, space);
}
     
void shockwaveEffect(Planet* planet, Space* space) {
	planet->dyingEffect(planet, space); 
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->shockwaves[i]->active) {
			space->shockwaves[i] = initShockwave(planet->x, planet->y, 10.0, 0, 1, planet->rgb);
			break;
		}
	}
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->shockwaves[i]->active) {
			space->shockwaves[i] = initShockwave(planet->x, planet->y, 9.0, 0, 1, planet->rgb);
			break;
		}
	}
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->shockwaves[i]->active) {
			space->shockwaves[i] = initShockwave(planet->x, planet->y, 7.0, 0, 1, planet->rgb);
			break;
		}
	}
}

void drawBlackHoll(BlackHoll *blackHoll) {
	int* rgb;
	int rgbArr[5][3] = {{100, 30, 90},{150, 60, 60},{200, 90, 30},{255, 120, 0},{255, 255, 255}};

	for (int i = 0; i <5; i++) {
		int p = (6-i) * 2;
		rgb = createRGB(rgbArr[i][0], rgbArr[i][1], rgbArr[i][2]);
		drawCircle(blackHoll->x - blackHoll->size - p, blackHoll->y - blackHoll->size - p, blackHoll->x + blackHoll->size + p, blackHoll->y + blackHoll->size + p, rgb, 0);
	}

	drawCircle(blackHoll->x - blackHoll->size, blackHoll->y - blackHoll->size, blackHoll->x + blackHoll->size, blackHoll->y + blackHoll->size, blackHoll->rgb, 0);
	
	
}

Planet* initEarth(int x, int y, double size) {
	Planet* earth = (Planet*)malloc(sizeof(Planet));
	earth->x = x;
	earth->y = y;
	earth->size = size;
	earth->velocity = 0.05;
	earth->angle = 0.0;
	earth->radius = 100.0;
	return earth;
}

Planet* initPlanet(int x, int y, double size, double velocity, double radius, double angle, bool active, int direction, int type, int timer,
	void blackhollEffect(Planet* planet, Space* space), void collisionEffect(Planet* planet, Space* space), void dyingEffect(Planet* planet, Space* space), int r, int g, int b) {

	Planet* planet = (Planet*)malloc(sizeof(Planet));
	planet->x = x;
	planet->y = y;
	planet->size = size;
	planet->velocity = velocity;
	planet->radius = radius;
	planet->angle = angle;
	planet->active = active;
	planet->direction = direction;
	planet->type = type;
	planet->timer = timer;
	planet->centerX = WIDTH / 2;
	planet->centerY = HEIGHT / 2;
	planet->blackhollEffect = blackhollEffect;
	planet->collisionEffect = collisionEffect;
	planet->dyingEffect = dyingEffect;
	planet->rgb = createRGB(r, g, b);
	return planet;
}

Space* initSpace(int* orbitArr) {
	int i;
	Space* space = (Space*)malloc(sizeof(Space));

	space->orbit = orbitArr;
	space->planets = (Planet**)malloc(sizeof(Planet*) * MAX_PLANET);
	space->shockwaves = (Shockwave**)malloc(sizeof(Shockwave*) * MAX_PLANET);
	space->blackHolls = (BlackHoll**)malloc(sizeof(BlackHoll*) * MAX_PLANET);
	space->shockwaves[0] = initShockwave(0, 0, 0, 0, 0, nullptr);
	space->blackHolls[0] = initBlackHoll(WIDTH / 2, HEIGHT / 2, 64, 1, -1,smallerEffectBlackHoll);
	space->planets[0] = initPlanet(400, 400, 20.0, 0.05, 300.0, 0.0, 1, -1, 9, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 0, 200, 0);
	for (i = 1; i < MAX_PLANET; i++) {
		space->planets[i] = initPlanet(0, 0, 0.0, 0, 0, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, 0, 0, 0);
		space->shockwaves[i] = initShockwave(0, 0, 0, 0, 0, nullptr);
		space->blackHolls[i] = initBlackHoll(0, 0, 0, 0, -1, nullptr);
	}

	space->earth = space->planets[0];
	space->mainBlackHoll = space->blackHolls[0];
	space->score = 0.0;
	return space;
}

void drawPlanet(Planet *planet) {
	drawCircle(planet->x - planet->size, planet->y - planet->size, planet->x + planet->size, planet->y + planet->size, planet->rgb, 0);
}

void drawOrbits(Space* space) {
	for (int i = 0; i < ORBIT; i++) {
		int* rgb = createRGB(200 - 20 * i, 200 - 20 * i, space->currentOrbit == i ? 200 - 20 * i + 50 : 200 - 20 * i);
		drawCircle(space->mainBlackHoll->x - space->orbit[i], space->mainBlackHoll->y - space->orbit[i], space->mainBlackHoll->x + space->orbit[i], space->mainBlackHoll->y + space->orbit[i], rgb, 0);
		free(rgb);
	}
}

void turnEarth(Planet* earth) {
	earth->angle += earth->velocity * (double)earth->direction;
	earth->angle = fmod(earth->angle, 360);
	earth->x = earth->radius * cos(earth->angle) + earth->centerX;
	earth->y = earth->radius * sin(earth->angle) + earth->centerY;
}

void controlEarth(Planet* earth, int* currentOrbit) {
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && earth->size == 20.0) {
		if (earth->radius < 500) {
			earth->radius += 0.4;
		}
	}
	else {
 		if (earth->radius > 0) {
			earth->radius -= 0.4;
		}
	}

	if (earth->radius >= 0 && earth->radius < 120) {
		earth->velocity = 0.012;
		*currentOrbit = 3;
	}
	else if (earth->radius >= 100 && earth->radius < 200) {
		earth->velocity = 0.008;
		*currentOrbit = 2;
	}
	else if (earth->radius >= 200 && earth->radius < 300) {
		earth->velocity = 0.0055;
		*currentOrbit = 1;
	}
	else if (earth->radius >= 300 && earth->radius < 400) {
		earth->velocity = 0.003;
		*currentOrbit = 0;
	}

}


void cls(int text_color, int bg_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "mode con:cols=110 lines=58 | COLOR %x%x", bg_color, text_color);
	system(cmd);

}



Planet* initRandomPlanet() {
	int random = rand()%5+1;
	double angle = rand() % 36000;
	angle /= 100.0;
	switch (random) {
	case 1:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 16.0, 0.3, 500, angle, 1, 1, 1, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 200, 0, 0);
	case 2:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 10.0, 0.4, 500, angle, 1, 1, 2, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 200, 200, 0);
	case 3:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 26.0, 0.2, 500, angle, 1, 1, 3, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 200, 0, 200);
	case 4:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 15.0, 0.3, 500, angle, 1, 1, 4, 0, shockwaveEffect, shockwaveEffect, dyingEffect4, 0, 200, 200);
	case 5:
		int timer = rand() % 300 + 100;
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 18.0, 0.25, 500, angle, 1, 1, 5, timer, shockwaveEffect, shockwaveEffect, dyingEffect5, 50, 50, 50);
	}

}

void movePlanet(Planet *planet) {
	planet->radius -= planet->velocity;
	planet->x = cos(planet->angle) * planet->radius + planet->centerX;
	planet->y = sin(planet->angle) * planet->radius + planet->centerY;
}

void progressPlanets(Planet** planets) {           
	double angle;
	for (int i = 1; i < MAX_PLANET; i++) {
		if (!(planets[i]->active)) { 
			if (fps % 300 == 0) {
				planets[i] = initRandomPlanet();
				cnt++;
				fps++;
			}
			else continue;
		}
		else {
			movePlanet(planets[i]);
			drawPlanet(planets[i]);
		}
	}
}   

double calculateDistance(int x1, int x2, int y1, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); 
}

// calculateDistance(space->planets[i]->x, space->blackHolls[j]->x, space->planets[i]->y, space->blackHolls[j]->y)
void planetCollision(Space* space) {
	if ((space->planets[0]->y - space->blackHolls[0]->y)) {
		gotoxy(10, 0);
		printf("%lf", atan2((space->planets[0]->x - space->blackHolls[0]->x) , (space->planets[0]->y - space->blackHolls[0]->y)));
	}
	
	
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->planets[i]->active) continue; 
		for (int j = 0; j < MAX_PLANET; j++) {
			if (!space->blackHolls[j]->active) continue;
			if (calculateDistance(space->planets[i]->x, space->blackHolls[j]->x, space->planets[i]->y, space->blackHolls[j]->y) < (space->blackHolls[j]->size) + (space->planets[i]->size) - 4) { // 행성과 블랙홀 충돌
				space->planets[i]->centerX = space->blackHolls[j]->x;
				space->planets[i]->centerY = space->blackHolls[j]->y;
				space->planets[i]->radius = calculateDistance(space->planets[i]->x, space->blackHolls[j]->x, space->planets[i]->y, space->blackHolls[j]->y);
				space->planets[i]->angle = atan2((space->planets[i]->y - space->blackHolls[j]->y) , (space->planets[i]->x - space->blackHolls[j]->x));

				space->planets[i]->blackhollEffect(space->planets[i], space);
			}
			if (!i) continue;
			if (calculateDistance(space->planets[i]->x, space->earth->x, space->planets[i]->y, space->earth->y) < (space->earth->size) + (space->planets[i]->size) - 4) { // 행성과 지구 충돌
				space->earth->collisionEffect(space->earth, space);
				space->planets[i]->collisionEffect(space->planets[i], space);
				//earth->active = false;
			}
		}
		if (space->planets[i]->type == 5 && (int)space->planets[i]->radius == space->planets[i]->timer) {
			space->planets[i]->blackhollEffect(space->planets[i], space);
		}
	}
}

void progressBlackHoll(BlackHoll** blackHolls) {
	for (int i = 0; i < MAX_PLANET; i++) {
		if (blackHolls[i]->active) {
			drawBlackHoll(blackHolls[i]);
			if (!blackHolls[i]->timer) blackHolls[i]->dyingEffect(blackHolls[i]);
			if (blackHolls[i]->timer > 0) blackHolls[i]->timer--;
		}
	}
}

void drawScore(double score) {
	int intScore = score;
	TCHAR ipOut[1024];
	int num = 1;
	wsprintf(ipOut, TEXT("%d"),intScore);
	TextOut(hdc, WIDTH/2, HEIGHT/2, ipOut, lstrlen(ipOut));
	//LPCWSTR str = (TEXT("%lf"),score);
	//RECT rt = { WIDTH/2-20, HEIGHT/2-20, WIDTH/2+20, HEIGHT/2+20 };
	//DrawText(hdc, str, -1, &rt, DT_CENTER | DT_WORDBREAK);

}

void progressSpace(Space *space) {
	progressBlackHoll(space->blackHolls);
	controlEarth(space->earth, &space->currentOrbit);
	turnEarth(space->earth);
	drawPlanet(space->earth);
	drawScore(space->score);
	space->score += space->earth->velocity;
	progressShockwave(space->shockwaves);

	planetCollision(space);
	progressPlanets(space->planets);
	if (space->earth->active == false) exit(1);
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

int main() {
	int Delay = 10;
	unsigned char ch;


	cls(0, 0);

	int orbitArr[ORBIT] = { 400, 300, 200, 120 };
	Space* space = initSpace(orbitArr);

	srand(time(NULL));

	Sleep(10);  


	while (1) {


		startBuffer();

		//gotoxy(10, 0);
		//printf("%d", cnt); 

		drawOrbits(space);
		progressSpace(space);
		

		endBuffer();
		fps++;
		Sleep(1);
	}




	getch();

	return 0;
}