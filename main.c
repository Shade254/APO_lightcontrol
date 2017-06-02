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


#define MZAPO 1

#define RED 0
#define GREEN 1
#define BLUE 2

#define INCREMENT_TYPE 1
#define SET_TYPE 2

#define CAST_TIME 1
#define RESEARCH_TIME 10

#define SAMPLE_SIZE 8
#define START_SAMPLE 3

struct timespec loop_delay = {.tv_sec = 1, .tv_nsec = 1000 * 1000 * 1000};

unsigned char* mem_base;
unsigned char* lcd_base;
unsigned char* thisWalls;
unsigned char* thisCeiling;
char* thisText;

unsigned long lastBroadcast = 0;
unsigned long lastResearch = 0;


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
	unsigned char* newValues;
	int state = 0;
	int changed = 0;
	uint32_t new = getKnobsValue(mem_base);
	oldValues = numToCharRGB(new);
	
	while(1){
		new = getKnobsValue(mem_base);
		
		int* b = getButtonValue(new);
		if(b[RED]) break;
		if(b[GREEN]) flag = !flag;
		if(b[BLUE]){
			 state++;
			 state=state%3;
		}
		
		if(state == 2){
			for(int i = 0;i<3;i++){
				wallRGB[i] = 0;
				ceilingRGB[i] = 0;
			}
			sendEdit(socket, wallRGB, ceilingRGB, ip, SET_TYPE);
			changed = 0;
		} else{
			newValues = numToCharRGB(new);
			for(int i = 0;i<3;i++){
				int increment = getIndexIncrement(oldValues[i], newValues[i]);
				if(increment!=0) changed=1;
			
				if(!flag){
					wallRGB[i] += increment;
				}
				else{
					ceilingRGB[i] += increment;
				}
			}
		} 
		
		if(changed){
			if(state == 1){
				unsigned char* pomWall = calloc(3, sizeof(char));
				unsigned char* pomCeiling = calloc(3, sizeof(char));
				unsigned char* origWall = numToCharRGB(message->wallsRGB);
				unsigned char* origCeiling = numToCharRGB(message->ceilingRGB);
				
				for(int i = 0;i<3;i++){
					pomWall[i] = wallRGB[i] - origWall[i];
					pomCeiling[i] = ceilingRGB[i] - origCeiling[i];
				}
				sendEdit(socket, pomWall, pomCeiling, ip, INCREMENT_TYPE);
				message->wallsRGB = charToNumRGB(wallRGB);
				message->ceilingRGB = charToNumRGB(ceilingRGB);
			}
			
			else{
				sendEdit(socket, wallRGB, ceilingRGB, ip, SET_TYPE);
			}
		}
		
		Image* img = createDetailScreen(ip, flag, wallRGB, ceilingRGB, message);
		
		if(state == 2){
			writeText(img, 185, 220, "Lights OFF");
		} else if(state == 1){
			writeText(img, 170, 220, "Increment format");
		} else if(state == 0){
			writeText(img, 160, 220, "Direct set format");
		}
		
		repaintScreen(lcd_base, img);
		free(img);
		free(oldValues);
		oldValues = newValues;
		broadcastMe(socket);
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
}

int init(){
	int socket = initCommunication();
	if(socket == 0) return 0;
	
	if(MZAPO){
		lcd_base = initScreen();
		mem_base = initMemBase();
		if(lcd_base == NULL || mem_base == NULL) return 0;
	}
	
	thisCeiling = calloc(3, sizeof(char));
	thisWalls = calloc(3, sizeof(char));
	thisText = "shade";
	
	thisWalls[RED] = 12;
	thisWalls[GREEN] = 14;
	thisWalls[BLUE] = 122;
	
	
	thisCeiling[RED] = 0;
	thisCeiling[GREEN] = 255;
	thisCeiling[BLUE] = 16;
	
	thisImage = malloc(512);
	
	if(thisCeiling == NULL || thisWalls == NULL || thisImage == NULL)
		return 0;
	
	memcpy(thisImage, mario, 512);
	return socket;
}

void broadcastMe(int socket){
	if((time(NULL) - lastBroadcast) >= CAST_TIME){
		printf("Broadcasting...\n");
		int b = broadcastInfo(socket, thisWalls, thisCeiling, thisText, thisImage);
		printf("%d\n", b);
		if(b == 1){
			lastBroadcast = time(NULL);
		}
	}
}


int main(){
	
	int socket = init(MZAPO);
	Image* img;
	unsigned char* inputValues;
	int lastInput;
	int index = 0;
	
	AreaInfo* areaInfo = calloc(1, sizeof(AreaInfo));
	char** labels = calloc(1, sizeof(char*));
	
	
	if(socket == 0){
		printf("[ERROR] Init failed\n");
		exit(1);
	} else{
		printf("[OK] Init\n");
	}
		
	
	if(socket != 0){
		printf("[OK] Socket valid!\n");
	} else{
		printf("[ERROR] Socket cannot be inicialized!\n");
		exit(1);
	}
	
	
	if(MZAPO){
		inputValues = numToCharRGB(getKnobsValue(mem_base));
		lastInput = (int)inputValues[BLUE];
		free(inputValues);
		img = createResearchScreen();
		repaintScreen(lcd_base, img);
		areaInfo = getBroadcasters(socket, START_SAMPLE);
		labels = calloc(areaInfo->size, sizeof(char*));			
			for(int i = 0;i<areaInfo->size;i++){
					labels[i] = areaInfo->messages[i]->text;
			}
			
		lastResearch = time(NULL);	
	}
	
	
	
	while(MZAPO){
		uint32_t knobs = getKnobsValue(mem_base);
		int* buttons = getButtonValue(knobs);
			
			
		if((time(NULL)-lastResearch) >= RESEARCH_TIME){
			printf("Researching area\n");
			areaInfo = getBroadcasters(socket, SAMPLE_SIZE);
			labels = calloc(areaInfo->size, sizeof(char*));			
			for(int i = 0;i<areaInfo->size;i++){
					labels[i] = areaInfo->messages[i]->text;
			}
			
			lastResearch = time(NULL);			
		}	
		
		
		if(buttons[GREEN]){
			runSettings(areaInfo->ips[index], mem_base, lcd_base, socket, areaInfo->messages[index]);
		} 
		
		if(buttons[RED]){
			exit(1);
		}
		
		inputValues = numToCharRGB(getKnobsValue(mem_base));
		index += getIndexIncrement(lastInput, (int)inputValues[BLUE]);
		
		if(index<0) index = (5+index);
		index = index%areaInfo->size;
		lastInput = (int)inputValues[BLUE];
		
		img = createMenuScreen(areaInfo, index);
		repaintScreen(lcd_base, img);
		
		free(img);
		free(inputValues);
		free(buttons);
		
		broadcastMe(socket);
		
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
	
	if(!MZAPO){
		img = createResearchScreen();
		showScreen("research.ppm", img);
		printf("--------------------Getting broadcasters----------------\n");
		areaInfo = getBroadcasters(socket, SAMPLE_SIZE);
		printf("--------------------Received----------------\n");
		for(int i = 0;i<areaInfo->size;i++){
			printf("%s - %d\n", areaInfo->messages[i]->text, numToCharRGB(areaInfo->messages[i]->wallsRGB)[0]);
		}
		areaInfo = sortAreaByName(areaInfo);
		
		printf("--------------------Sorted----------------\n");
		for(int i = 0;i<areaInfo->size;i++){
			printf("%s - %d\n", areaInfo->messages[i]->text, numToCharRGB(areaInfo->messages[i]->wallsRGB)[0]);
		}
		
		printf("---------------------Painting---------------\n");
		img = createMenuScreen(areaInfo, 0);
		showScreen("area.ppm", img);
		
		img = createDetailScreen(areaInfo->ips[0], 1, numToCharRGB(areaInfo->messages[0]->wallsRGB), numToCharRGB(areaInfo->messages[0]->ceilingRGB), areaInfo->messages[0]);
		writeText(img, 160, 220, "Direct set format");
		
		showScreen("detail.ppm", img);
	}
	
	
	
	
	return 0;
}



