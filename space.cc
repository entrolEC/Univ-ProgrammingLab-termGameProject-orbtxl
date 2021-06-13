#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "space.h"
#include "planet.h"
#include "effect.h"
#include "shockwave.h"
#include "draw.h"

extern int done;
extern int MAX_PLANET, WIDTH, HEIGHT, ORBIT;

Space* initSpace(int* orbitArr) {
	int i;
	Space* space = (Space*)malloc(sizeof(Space));

	space->orbit = orbitArr;
	space->planets = (Planet**)malloc(sizeof(Planet*) * MAX_PLANET);
	space->shockwaves = (Shockwave**)malloc(sizeof(Shockwave*) * MAX_PLANET);
	space->blackHolls = (BlackHoll**)malloc(sizeof(BlackHoll*) * MAX_PLANET);
	space->shockwaves[0] = initShockwave(0, 0, 0, 0, 0, nullptr);
	space->blackHolls[0] = initBlackHoll(WIDTH / 2, HEIGHT / 2, 60, 1, -1, smallerEffectBlackHoll);
	space->planets[0] = initPlanet(500, 500, 18.0, 0.05, 300.0, 0.0, 1, -1, 9, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 0, 200, 0);
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

void gameOver(Space* space) {
	space->planets[0]->collisionEffect(space->planets[0], space, space->blackHolls[1]);
	for (int i = 1; i < MAX_PLANET; i++) {
		if (space->planets[i]->active) {
			space->planets[i]->active = false; // 6锅 青己父 贸府瞪巴烙
			space->planets[i]->collisionEffect(space->planets[i], space, space->blackHolls[1]);
		}

		if (space->blackHolls[i]->active)
			space->blackHolls[i]->dyingEffect(space->blackHolls[i]);
	}
	done = 1;
	return;
}

void drawOrbits(Space* space) {
	for (int i = 0; i < ORBIT; i++) {
		int* rgb = createRGB(200 - 20 * i, 200 - 20 * i, space->currentOrbit == i ? 200 - 20 * i + 50 : 200 - 20 * i);
		drawCircle(space->mainBlackHoll->x - space->orbit[i], space->mainBlackHoll->y - space->orbit[i], space->mainBlackHoll->x + space->orbit[i], space->mainBlackHoll->y + space->orbit[i], rgb, 0);
		free(rgb);
	}
}

int orbitVelocity(Planet* planet) {

	if (planet->radius >= 0 && planet->radius < 120) {
		planet->velocity = 0.02;
		return 3;
	}
	else if (planet->radius >= 100 && planet->radius < 200) {
		planet->velocity = 0.014;
		return 2;
	}
	else if (planet->radius >= 200 && planet->radius < 300) {
		planet->velocity = 0.01;
		return 1;
	}
	else if (planet->radius >= 300 && planet->radius <= 405) {
		planet->velocity = 0.006;
		return 0;
	}
}

void planetCollision(Space* space) {
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->planets[i]->active) continue;
		for (int j = 0; j < MAX_PLANET; j++) {
			if (!space->blackHolls[j]->active) continue;
			if (calculateDistance(space->planets[i]->x, space->blackHolls[j]->x, space->planets[i]->y, space->blackHolls[j]->y) < (space->blackHolls[j]->size) + (space->planets[i]->size) - 4) { // 青己苞 喉发圈 面倒
				space->planets[i]->centerX = space->blackHolls[j]->x;
				space->planets[i]->centerY = space->blackHolls[j]->y;
				space->planets[i]->radius = calculateDistance(space->planets[i]->x, space->blackHolls[j]->x, space->planets[i]->y, space->blackHolls[j]->y);
				space->planets[i]->angle = (double)atan2((space->planets[i]->y - space->blackHolls[j]->y), (space->planets[i]->x - space->blackHolls[j]->x));

				space->planets[i]->blackhollEffect(space->planets[i], space, space->blackHolls[j]);
				i++;
			}
			if (!i) continue;
			if (calculateDistance(space->planets[i]->x, space->earth->x, space->planets[i]->y, space->earth->y) < (space->earth->size) + (space->planets[i]->size) - 4) { // 青己苞 瘤备 面倒
				space->earth->collisionEffect(space->earth, space, space->blackHolls[j]);
				space->planets[i]->collisionEffect(space->planets[i], space, space->blackHolls[j]);
				//earth->active = false;
			}
		}
		if (space->planets[i]->type == 5 && (int)space->planets[i]->radius == space->planets[i]->timer) {
			space->planets[i]->blackhollEffect(space->planets[i], space, nullptr);
		}
	}
}

void progressSpace(Space* space) {
	if (done) done++;
	if (done > 150) {
		Sleep(1000);
		exit(1);
	}
	progressBlackHoll(space->blackHolls);
	controlEarth(space->earth, &space->currentOrbit);
	turnEarth(space->earth);
	if (space->earth->active)
		drawPlanet(space->earth);
	drawScore(space->score);
	space->score += space->earth->velocity;
	progressShockwave(space->shockwaves);
	if (!done)
		planetCollision(space);
	progressPlanets(space->planets);
	if (!space->earth->active && !done) {
		gameOver(space);
	}
}