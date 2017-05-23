#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include "imgwrite.h"
#include "messages.h"
Image* createMenuScreen(char** names, int numOfClients,int index);

Image* createDetailScreen(char* ip, int index, unsigned char* wallsRGB, unsigned char* ceilingRGB, InfoMessage* message);

Image* createResearchScreen();

#endif
