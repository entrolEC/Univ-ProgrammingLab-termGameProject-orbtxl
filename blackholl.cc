#include <stdlib.h>
#include "blackholl.h"
#include "draw.h"

extern int MAX_PLANET;

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

void drawBlackHoll(BlackHoll* blackHoll) {
	int* rgb;
	int rgbArr[5][3] = { {100, 30, 90},{150, 60, 60},{200, 90, 30},{255, 120, 0},{255, 255, 255} };

	for (int i = 0; i < 5; i++) {
		int p = (6 - i) * 2;
		rgb = createRGB(rgbArr[i][0], rgbArr[i][1], rgbArr[i][2]);
		drawCircle(blackHoll->x - blackHoll->size - p, blackHoll->y - blackHoll->size - p, blackHoll->x + blackHoll->size + p, blackHoll->y + blackHoll->size + p, rgb, 0);
		free(rgb);
	}

	drawCircle(blackHoll->x - blackHoll->size, blackHoll->y - blackHoll->size, blackHoll->x + blackHoll->size, blackHoll->y + blackHoll->size, blackHoll->rgb, 0);


}

void progressBlackHoll(BlackHoll** blackHolls) {
	drawBlackHoll(blackHolls[0]);
	if (blackHolls[0]->size > 60.0) blackHolls[0]->size -= 0.03;
	for (int i = 1; i < MAX_PLANET; i++) {
		if (blackHolls[i]->active) {
			drawBlackHoll(blackHolls[i]);
			if (blackHolls[i]->timer <= 0) blackHolls[i]->dyingEffect(blackHolls[i]);
			if (blackHolls[i]->timer > 0) blackHolls[i]->timer--;
			if (blackHolls[i]->size > 25.0) blackHolls[i]->size -= 0.03;
		}
	}
}