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
#include "control.h"
#include "screen.h"
#include "utils.h"
#include "graphics.h"
#include "intercom.h"


#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255
#define INCREMENT_TYPE 1
#define SET_TYPE 2
#define MZAPO 1
#define CAST_TIME 1
#define RESEARCH_TIME 10

struct timespec loop_delay = {.tv_sec = 1, .tv_nsec = 1000 * 1000 * 1000};

unsigned char* mem_base;
unsigned char* lcd_base;
unsigned char* thisWalls;
unsigned char* thisCeiling;
char* thisText;
int16_t* thisImage;

int16_t mario[256] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0xFDEF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0xFDEF,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0xFDEF,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0xFDEF,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0xFDEF,0xFDEF,0xFDEF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0x5014,0xF800,0xF800,0xF800,0x5014,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0x5014,0xF800,0xF800,0x5014,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0x5014,0x5014,0x5014,0x5014,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFDEF,0xFDEF,0xF800,0x5014,0xFFE5,0x5014,0x5014,0xFFE5,0x5014,0xF800,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFDEF,0xFDEF,0xFDEF,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0xFDEF,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFDEF,0xFDEF,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x5014,0x5014,0x5014,0xFFFF,0xFFFF,0x5014,0x5014,0x5014,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,
};

void runSettings(char* ip, unsigned char* mem_base, unsigned char* lcd_base, int socket, InfoMessage* message){
	unsigned char wallRGB[3] = {0, 0, 0};
	unsigned char ceilingRGB[3] = {0, 0, 0};
	int flag = 0;
	unsigned char* oldValues;
	uint32_t new = getKnobsValue(mem_base);
	oldValues = numToCharRGB(new);
	
	while(1){
		new = getKnobsValue(mem_base);
		
		int* b = getButtonValue(new);
		if(b[0]) break;
		if(b[1]) flag = !flag;
		if(b[2]){
			 sendEdit(socket, wallRGB, ceilingRGB, ip);
			 break;
		 }
		 
		
		unsigned char* newValues = numToCharRGB(new);
		
		for(int i = 0;i<3;i++){
			if(!flag){
				wallRGB[i] += getIndexIncrement(oldValues[i], newValues[i]);
			}
			else{
				ceilingRGB[i] += getIndexIncrement(oldValues[i], newValues[i]);
			}
		}
		
		Image* img = createDetailScreen(ip, flag, wallRGB, ceilingRGB, message);
		repaintScreen(lcd_base, img);
		free(img);
		free(oldValues);
		oldValues = newValues;

		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
}

int init(int mzapo){
	int socket = initCommunication();
	if(socket == 0) return 0;
	
	if(mzapo){
		lcd_base = initScreen();
		mem_base = initMemBase();
		if(lcd_base == NULL || mem_base == NULL) return 0;
	}
	
	thisCeiling = calloc(3, sizeof(char));
	thisWalls = calloc(3, sizeof(char));
	thisText = "...---...";
	
	thisWalls[0] = 12;
	thisWalls[1] = 14;
	thisWalls[2] = 122;
	
	
	thisCeiling[0] = 0;
	thisCeiling[1] = 255;
	thisCeiling[2] = 16;
	
	thisImage = malloc(512);
	
	if(thisCeiling == NULL || thisWalls == NULL || thisImage == NULL)
		return 0;
	
	memcpy(thisImage, mario, 512);
	return socket;
}

int main(){
	
	int socket = init(MZAPO);
	if(socket == 0){
		printf("[ERROR] Init failed\n");
		exit(1);
	} else{
		printf("[OK] Init\n");
	}
	
	unsigned long lastBroadcast = 0;
	unsigned long lastResearch = 0;
	
	Image* img;
	
	if(socket != 0){
		printf("[OK] Socket valid!\n");
	} else{
		printf("[ERROR] Socket cannot be inicialized!\n");
		exit(1);
	}
	
	int index = 0;
	unsigned char* val = numToCharRGB(getKnobsValue(mem_base));
	int lastVal = (int)val[0];
	free(val);

	AreaInfo* info = calloc(1, sizeof(AreaInfo));
	char** labels = calloc(1, sizeof(char*));
	
	while(MZAPO){
		uint32_t knobs = getKnobsValue(mem_base);
		int* buttons = getButtonValue(knobs);
			
			
		if((time(NULL)-lastResearch) >= RESEARCH_TIME){
			img = createResearchScreen();
			repaintScreen(lcd_base, img);
			printf("Researching area\n");
			info = getBroadcasters(socket, 12);
			labels = calloc(info->size, sizeof(char*));			
			for(int i = 0;i<info->size;i++){
					labels[i] = info->messages[i]->text;
			}
			lastResearch = time(NULL);			
		}	
		
		
		if(buttons[1]){
			runSettings(info->ips[index], mem_base, lcd_base, socket, info->messages[index]);
		} 
		
		if(buttons[0]){
			exit(1);
		}
		
		val = numToCharRGB(getKnobsValue(mem_base));
		index += getIndexIncrement(lastVal, (int)val[0]);
		
		if(index<0) index = (5+index);
		index = index%info->size;
		lastVal = (int)val[0];
		
		img = createMenuScreen(labels, info->size, index);
		repaintScreen(lcd_base, img);
		
		free(img);
		free(val);
		free(buttons);
		
		if((time(NULL) - lastBroadcast) >= CAST_TIME){
			printf("Broadcasting...\n");
			int b = broadcastInfo(socket, thisWalls, thisCeiling, thisText, thisImage);
			printf("%d\n", b);
			if(b == 1){
				lastBroadcast = time(NULL);
			}
		}
		
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
	
	return 0;
}



