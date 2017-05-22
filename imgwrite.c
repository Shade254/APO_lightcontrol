#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font_types.h"
#include "imgwrite.h"

#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255

void showScreen(char* filename, Image* img){
	FILE* fp = fopen(filename, "wb");

	if(!fp) exit(1);
	fprintf(fp, "P6 %d %d %d\n", img->x, img->y, RGB_MAX);
	fwrite(img->data, 3*img->x, img->y, fp);
	fclose(fp);
}

Image* createTextScreen(int posx, int posy, char* text){
	Image* img = createBlankScreen(BASE_R, BASE_G, BASE_B);
	writeText(img, posx, posy, text);
	return img;
}

Image* createBlankScreen(int r, int g, int b){
	Image* img = malloc(sizeof(*img));
	
	if(!img) exit(1);
	
	img->x = DIM_X;
	img->y = DIM_Y;
	
	img->data = calloc(img->x*img->y, sizeof(Pixel));
	if(!img->data) exit(1);
	
	for(int i = 0;i<(img->x*img->y);i++){
		img->data[i].red = r;
		img->data[i].blue = b;
		img->data[i].green = g;
	}
	
	return img;
}

Image* setPixel(Image* img, int x, int y, int r, int g, int b){
	if(!img) exit(1);
	
	if(x>img->x||x<0||y>img->y||y<0){
		fprintf(stderr, "Wrong dim - setPixel()\n");
		return img;
	}
	int i = (y*img->x) + x;;
	
	img->data[i].red = r;
	img->data[i].green = g;
	img->data[i].blue = b;
	
	return img;
}


Image* paintPixel(Image* img, int x, int y, int16_t color){
	unsigned r = (color & 0xF800) >> 8;      
	unsigned g = (color & 0x07E0) >> 3;      
	unsigned b = (color & 0x1F) << 3; 
	return setPixel(img, x, y, r, g, b);
}


Image* writeText(Image* img, int posx, int posy, char* text){
	int length = strlen(text);
	int x = posx;	
	int y = posy;
	for(int i = 0;i<length;i++){
		
		if((int)text[i] == 10){
			y += font_rom8x16.height;
			x = posx;
			continue;
		}
		
		writeLetter(img, x, y, text[i]);
		x += font_rom8x16.maxwidth;
	}
	return img;
}

Image* writeLetter(Image* img, int x, int y, unsigned char c){
	int offset = ((int)c) * 16;
	
	for(int i = 0;i<font_rom8x16.height;i++){
		uint16_t line = font_rom8x16.bits[offset+i];	
			
		for(int j = 0;j<font_rom8x16.maxwidth;j++){
			uint16_t mask = 1;
			mask = mask<<15;
			mask = mask>>j;
			if((line&mask)){
				setPixel(img, x+j, y+i, TEXT_R, TEXT_G, TEXT_B);
			}
		}
	}
	
	
	return img;
}
