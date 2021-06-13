#include <stdlib.h>
#include "shockwave.h"
#include "draw.h"

extern int MAX_PLANET;

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

void drawShockwave(Shockwave* shockwave) {
	drawCircle(shockwave->x - shockwave->size, shockwave->y - shockwave->size, shockwave->x + shockwave->size, shockwave->y + shockwave->size, shockwave->rgb, 1);
}

void progressShockwave(Shockwave** shockwaves) {
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!shockwaves[i]->active) continue;
		if (shockwaves[i]->size > 1000) shockwaves[i]->active = false;
		shockwaves[i]->size *= 1.02;
		drawShockwave(shockwaves[i]);
	}
}