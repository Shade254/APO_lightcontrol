#ifndef IMGWRITE_H_
#define IMGWRITE_H_

#define RGB_MAX 255

typedef struct {
	unsigned char red, green, blue;
} Pixel;

typedef struct {
	int x, y;
	Pixel* data;
} Image;

void showScreen(char* filename, Image* img);

Image* createTextScreen(int posx, int posy, char* text);

Image* createBlankScreen(int r, int g, int b);

Image* writeText(Image* img, int x, int y, char* text);

Image* setPixel(Image* img, int x, int y, int r, int g, int b);

Image* writeLetter(Image* img, int x, int y, unsigned char c);



#endif
