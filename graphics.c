#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "intercom.h"
#include "graphics.h"
#include "imgwrite.h"
#include "messages.h"
#include "utils.h"


#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255

Image* createMenuScreen(AreaInfo* area, int index){
	Image* img = createBlankScreen(BASE_R, BASE_G, BASE_B);
	
	int line = 0;
	writeText(img, 20, 20*line, "Available units: ");
	line++;
	for(int i = 0;i<area->size;i++){
		if(index == i){
			char pom[2] = {16, 0};
			writeText(img, 0, 20*line, pom);
		}
		writeText(img, 20, 20*line, area->messages[i]->text);
		paintIcon(img, area->messages[i]->image, 16, 16, 120, 20*line);
		unsigned char* ceiling = numToCharRGB(area->messages[i]->ceilingRGB);
		unsigned char* walls = numToCharRGB(area->messages[i]->wallsRGB);
		
		char* pom = calloc(30, sizeof(char));
		sprintf(pom, "Walls:%03d/%03d/%03d", walls[0], walls[1], walls[2]);
		writeText(img, 150, 20*line, pom);
		sprintf(pom, "Ceiling:%03d/%03d/%03d", ceiling[0], ceiling[1], ceiling[2]);
		writeText(img, 300, 20*line, pom);
		line++;
	}
	
	writeText(img, 10, 300, "Quit");
	writeText(img, 210, 300, "Confirm");
	writeText(img, 390, 300, "Choose");
	return img;
}

Image* paintBar(Image* img, int posx, int posy, int x, int y, uint32_t rgb){
	for(int i = 0;i<x;i++){
		for(int j = 0;j<y;j++){
			unsigned char* color = numToCharRGB(rgb);
			setPixel(img, posx+i, posy+j, color[0], color[1], color[2]);
		}
	}
}

Image* paintGraph(Image* img, int posx, int posy, unsigned char* values){
	unsigned char* rgb = calloc(3, sizeof(char));
	char* pom = calloc(20, sizeof(char));
	
	for(int i = 0;i<3;i++){
		rgb[0] = 0;
		rgb[1] = 0;
		rgb[2] = 0;
		sprintf(pom, "%03d", values[i]);
		writeText(img, posx, posy + i*30, pom);
		if(i==0) rgb[0] = 255;
		if(i==1) rgb[1] = 255;
		if(i==2) rgb[2] = 255;
		paintBar(img, posx+30, posy+i*30, values[i]/2, 10, charToNumRGB(rgb));
	}
}

Image* createDetailScreen(char* ip, int index, unsigned char* wallsRGB, unsigned char* ceilingRGB, InfoMessage* message){
	char* pom = calloc(60, sizeof(char));
	char* title = calloc(55, sizeof(char));
	sprintf(title, "Settings of %s\n on adress %s", message->text, ip);
	char arrow[2] = {16,0};

	Image* img = createTextScreen(20, 20, title);
	writeText(img, 80, 60, "Walls");
	writeText(img, 320, 60, "Ceiling");
	paintGraph(img, 20, 80, wallsRGB);
	paintGraph(img, 250, 80, ceilingRGB);
	
	if(index == 0){
		writeText(img, 70, 60, arrow);
	} else {
		writeText(img, 310, 60, arrow);
	}
	
	writeText(img, 0, 300, "Back");
	writeText(img, 185, 300, "Toggle C/W");
	writeText(img, 370, 300, "Toogle modes");
	return img;
}

Image* paintIcon(Image* img, int16_t* rgb, int x, int y, int posx, int posy){
	for(int a = 0;a<y;a++){
		for(int b = 0;b<x;b++){
			paintPixel(img, posx+b, posy+a, rgb[(a*x + b)]);
		}
	}
	return img;
}

Image* createResearchScreen(){
	Image* img = createBlankScreen(BASE_R, BASE_G, BASE_B);
	
	writeText(img, 20, 0, "Available units: ");
	writeText(img, 190, 280, "Searching area...");
	
	return img;
}
