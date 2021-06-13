#pragma once
void drawCircle(int x1, int y1, int x2, int y2, int* rgb, bool clearCircle);
int* createRGB(int r, int g, int b);
void cls(int text_color, int bg_color);
double calculateDistance(int x1, int x2, int y1, int y2);
void drawScore(double score);

void startBuffer();
void endBuffer();