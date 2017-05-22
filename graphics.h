#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include "imgwrite.h"
Image* createMenuScreen(char** names, int numOfClients,int index);

Image* createDetailScreen(char* ip, char* name, int index, unsigned char* wallsRGB, unsigned char* ceilingRGB);

Image* createResearchScreen();

#endif
