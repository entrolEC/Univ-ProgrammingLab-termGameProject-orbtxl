#pragma once

struct Space;
struct BlackHoll;

struct Planet {
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
	void (*blackhollEffect)(Planet* planet, Space* space, BlackHoll* blackHoll);
	void (*collisionEffect)(Planet* planet, Space* space, BlackHoll* blackHoll);
	void (*dyingEffect)(Planet* planet, Space* space, BlackHoll* blackHoll);
	int* rgb;
};

Planet* initPlanet(int x, int y, double size, double velocity, double radius, double angle, bool active, int direction, int type, int timer,
	void blackhollEffect(Planet* planet, Space* space, BlackHoll* blackHoll), void collisionEffect(Planet* planet, Space* space, BlackHoll* blackHoll),
	void dyingEffect(Planet* planet, Space* space, BlackHoll* blackHoll), int r, int g, int b);

Planet* initRandomPlanet();

void drawPlanet(Planet* planet);

void turnEarth(Planet* earth);

void controlEarth(Planet* earth, int* currentOrbit);

void movePlanet(Planet* planet);

void progressPlanets(Planet** planets);