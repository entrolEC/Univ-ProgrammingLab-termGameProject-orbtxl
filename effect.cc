#include "space.h"
#include "blackholl.h"
#include "planet.h"
#include "shockwave.h"

extern int MAX_PLANET;

void doNoting(Planet* planet, Space* space, BlackHoll* blackHoll) {
	planet->dyingEffect(planet, space, blackHoll);
}

void smallerEffectBlackHoll(BlackHoll* blackHoll) {
	blackHoll->size *= 0.98;
	if (blackHoll->size <= 3) blackHoll->active = false;
}

void dyingEffect6(Planet* planet, Space* space, BlackHoll* blackHoll) {
	if (blackHoll->size < 120 && blackHoll->active) blackHoll->size *= 1.005;
	planet->size *= 0.98;
	if (planet->size <= 5)
		planet->active = false;

}

void dyingEffect5(Planet* planet, Space* space, BlackHoll* blackHoll) {
	planet->active = false;
	for (int i = 1; i < MAX_PLANET; i++) {
		if (!space->blackHolls[i]->active) {
			space->blackHolls[i] = initBlackHoll(planet->x, planet->y, 25.0, 1, 2000, smallerEffectBlackHoll);
			break;
		}
	}
}

void dyingEffect4(Planet* planet, Space* space, BlackHoll* blackHoll) {
	planet->active = false;
	space->earth->direction *= -1;
}

void dyingEffectNone(Planet* planet, Space* space, BlackHoll* blackHoll) {
	planet->active = false;
}

void smallerEffect(Planet* planet, Space* space, BlackHoll* blackHoll) {
	planet->size *= 0.98;
	planet->velocity *= 0.99;
	if (planet->size <= 5) planet->dyingEffect(planet, space, blackHoll);
}

void shockwaveEffect(Planet* planet, Space* space, BlackHoll* blackHoll) {
	planet->dyingEffect(planet, space, blackHoll);
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->shockwaves[i]->active) {
			space->shockwaves[i] = initShockwave(planet->x, planet->y, planet->size, 0, 1, planet->rgb);
			break;
		}
	}
	for (int i = 0; i < MAX_PLANET; i++) {
		if (!space->shockwaves[i]->active) {
			space->shockwaves[i] = initShockwave(planet->x, planet->y, planet->size - 2, 0, 1, planet->rgb);
			break;
		}
	}
}
