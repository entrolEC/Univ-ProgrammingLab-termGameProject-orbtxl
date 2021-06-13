#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include "planet.h"
#include "draw.h"
#include "space.h"
#include "effect.h"

extern int fps;
extern int WIDTH, HEIGHT, MAX_PLANET;

Planet* initPlanet(int x, int y, double size, double velocity, double radius, double angle, bool active, int direction, int type, int timer,
	void blackhollEffect(Planet* planet, Space* space, BlackHoll* blackHoll), void collisionEffect(Planet* planet, Space* space, BlackHoll* blackHoll), 
	void dyingEffect(Planet* planet, Space* space, BlackHoll* blackHoll), int r, int g, int b) {

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

Planet* initRandomPlanet() {
	int difficulty, timer, random;
	double angle = rand() % 36000;
	angle /= 100.0;
	if (fps < 5000) difficulty = 3;
	else if (fps < 10000) difficulty = 4;
	else if (fps < 15000) difficulty = 5;
	else if (fps < 20000) difficulty = 6;
	else if (fps < 30000) difficulty = 7;
	else difficulty = 7;
	random = rand() % difficulty + 1;
	switch (random) {
	case 1:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 16.0, 0.6, 500, angle, 1, 1, 1, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 200, 0, 0);
	case 2:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 10.0, 0.8, 500, angle, 1, 1, 2, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 255, 200, 0);
	case 3:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 24.0, 0.4, 500, angle, 1, 1, 3, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 200, 0, 200);
	case 4:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 22.0, 0.4, 500, angle, 1, 1, 6, timer, doNoting, shockwaveEffect, dyingEffect6, 50, 50, 50);
	case 5:
		timer = rand() % 300 + 100;
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 18.0, 0.5, 500, angle, 1, 1, 5, timer, shockwaveEffect, shockwaveEffect, dyingEffect5, 0, 0, 100);
	case 6:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 15.0, 0.6, 500, angle, 1, 1, 4, 0, shockwaveEffect, shockwaveEffect, dyingEffect4, 0, 200, 200);
	case 7:
		return initPlanet(cos(angle) * 500, sin(angle) * 500, 18.0, 0.007, 500, angle, 1, 1, 7, 0, smallerEffect, shockwaveEffect, dyingEffectNone, 200, 255, 50);
	}

}

void drawPlanet(Planet* planet) {
	drawCircle(planet->x - planet->size, planet->y - planet->size, planet->x + planet->size, planet->y + planet->size, planet->rgb, 0);
}

void turnEarth(Planet* earth) {
	earth->angle += earth->velocity * (double)earth->direction;
	earth->angle = fmod(earth->angle, 360);
	earth->x = earth->radius * cos(earth->angle) + earth->centerX;
	earth->y = earth->radius * sin(earth->angle) + earth->centerY;
}

void controlEarth(Planet* earth, int* currentOrbit) {
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && earth->size == 18.0) {
		if (earth->radius < 400) {
			earth->radius += 0.8;
		}
	}
	else {
		if (earth->radius > 0) {
			earth->radius -= 0.8;
		}
	}

	*currentOrbit = orbitVelocity(earth);
}

void movePlanet(Planet* planet) {
	planet->radius -= planet->velocity;
	planet->x = cos(planet->angle) * planet->radius + planet->centerX;
	planet->y = sin(planet->angle) * planet->radius + planet->centerY;
	if (planet->type == 7) {
		//planet->radius
		planet->radius -= planet->velocity * 10;
		orbitVelocity(planet);
		turnEarth(planet);
	}
}

void progressPlanets(Planet** planets) {
	double angle;
	for (int i = 1; i < MAX_PLANET; i++) {
		if (!(planets[i]->active)) {
			if (fps % 200 == 0) {
				planets[i] = initRandomPlanet();
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