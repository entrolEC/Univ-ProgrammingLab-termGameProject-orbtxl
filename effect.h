#pragma once

#include "planet.h"
#include "blackholl.h"
#include "shockwave.h"

void doNoting(Planet* planet, Space* space, BlackHoll* blackHoll);

void smallerEffectBlackHoll(BlackHoll* blackHoll);

void dyingEffect6(Planet* planet, Space* space, BlackHoll* blackHoll);

void dyingEffect5(Planet* planet, Space* space, BlackHoll* blackHoll);

void dyingEffect4(Planet* planet, Space* space, BlackHoll* blackHoll);

void dyingEffectNone(Planet* planet, Space* space, BlackHoll* blackHoll);

void smallerEffect(Planet* planet, Space* space, BlackHoll* blackHoll);

void shockwaveEffect(Planet* planet, Space* space, BlackHoll* blackHoll);
