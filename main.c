#define _POSIX_C_SOURCE 200112L

#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <byteswap.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "imgwrite.h"
#include "font_types.h"
#include "netcom.h"
#include "messages.h"
#include "control.h"
#include "screen.h"


#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255

Image* createMenuScreen(char** strings, int num, int index){
	Image* img = createBlankScreen(BASE_R, BASE_G, BASE_B);
	
	int line = 0;
	writeText(img, 20, 20*line, "Available units: ");
	line++;
	for(int i = 0;i<num;i++){
		if(index == i){
			char pom[2] = {16, 0};
			writeText(img, 0, 20*line, pom);
		}
		writeText(img, 20, 20*line, strings[i]);
		line++;
	}
	return img;
}

int getIndexIncrement(int lastVal, int curVal, int index){
	printf("Index %d (%d, %d) -> ", index, lastVal, curVal);
	if(lastVal != curVal){
		if(abs(curVal-lastVal) > 250){
			if(curVal>lastVal) return -1;
			if(lastVal>curVal) return +1;
		} else{
			if(curVal>lastVal) return +1;
			if(lastVal>curVal) return -1;
		}
	}
	return 0;
}

int main(){
	struct timespec loop_delay = {.tv_sec = 1, .tv_nsec = 1000 * 1000 * 1000};
	int socket = initCommunication();
	unsigned char* lcd_base = initScreen();
	unsigned char* mem_base = initMemBase();
	
	char* address = malloc(16*sizeof(char));
	char* pom = calloc(30, sizeof(char));
	const char* a[5];
	a[0] = "ahoj";
	a[1] = "lidi";
	a[2] = "jak";
	a[3] = "se";
	a[4] = "mate";
	
	Image* img;
	int line = 0;
	
	if(socket != 0){
		img = createTextScreen(20, line*20, "[INFO] Socket is valid");
	} else{
		img = createTextScreen(20, line*20, "[ERROR] Inicialization failed");
		exit(1);
	}
	
	repaintScreen(lcd_base, img);
	line++;
	
	int index = 0;
	unsigned char* val = numToCharRGB(getKnobsValue(mem_base));
	int lastVal = (int)val[0];
	
	free(img);
	while(1){
		uint32_t knobs = getKnobsValue(mem_base);
		int* buttons = getButtonValue(knobs);
		/*
		val = numToCharRGB(getKnobsValue(mem_base));
		index += getIndexIncrement(lastVal, (int)val[0], index);
		
		if(index<0) index = (5+index);
		index = index%5;
		printf("%d\n", index);
		
		lastVal = (int)val[0];
		*/
		sprintf(pom, "0x%08x\n%d %d %d\n", (unsigned int)knobs, buttons[0], buttons[1], buttons[2]);
		//img = createMenuScreen(a, 5, index);
		img = createTextScreen(20, 20, pom);
		repaintScreen(lcd_base, img);
		free(img);
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
	
	free(address);
	free(img);
	return 0;
}



