#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include "imgwrite.h"
#include "messages.h"
#include "intercom.h"
Image* createMenuScreen(AreaInfo* area, int index);

Image* createDetailScreen(char* ip, int index, unsigned char* wallsRGB, unsigned char* ceilingRGB, InfoMessage* message);

Image* createResearchScreen();

Image* paintIcon(Image* img, int16_t* rgb, int x, int y, int posx, int posy);

Image* paintGraph(Image* img, int posx, int posy, unsigned char* values);

Image* paintBar(Image* img, int posx, int posy, int x, int y, uint32_t rgb);

#endif
