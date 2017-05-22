#ifndef SCREEN_H_
#define SCREEN_H_
#include "imgwrite.h"

unsigned char* initScreen(); 

int rgbTo565(unsigned char r, unsigned char g, unsigned char b);

void repaintScreen(unsigned char* lcd_base, Image* screen);

#endif
