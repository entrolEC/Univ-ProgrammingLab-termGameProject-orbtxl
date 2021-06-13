#pragma once

struct Planet;
struct BlackHoll;
struct Shockwave;

struct Space {
	int* orbit;
	Planet* earth;
	BlackHoll* mainBlackHoll;
	Planet** planets;
	BlackHoll** blackHolls;
	Shockwave** shockwaves;
	int currentOrbit;
	double score;
};

Space* initSpace(int* orbitArr);

void gameOver(Space* space);

void drawOrbits(Space* space);

int orbitVelocity(Planet* planet);

void planetCollision(Space* space);

void progressSpace(Space* space);