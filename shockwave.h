#pragma once

struct Shockwave {
	int x;
	int y;
	double size;
	bool destructive;
	bool active;
	int* rgb;
};

Shockwave* initShockwave(int x, int y, double size, bool destructive, bool active, int* rgb);

void progressShockwave(Shockwave** shockwaves);