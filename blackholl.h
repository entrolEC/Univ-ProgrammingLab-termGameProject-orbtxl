#pragma once

struct BlackHoll {
	int x;
	int y;
	double size;
	bool active;
	int timer;
	void (*dyingEffect)(BlackHoll* blackHoll);
	int* rgb;
};

BlackHoll* initBlackHoll(int x, int y, double size, bool active, int timer, void dyingEffect(BlackHoll* blackHoll));

void drawBlackHoll(BlackHoll* blackHoll);

void progressBlackHoll(BlackHoll** blackHolls);