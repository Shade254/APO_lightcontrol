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

int main(){
	struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 2000 * 1000 * 1000};
	int socket = initCommunication();
	unsigned char* lcd_base = initScreen();
	unsigned char* mem_base = initMemBase();
	
	char* address = malloc(16*sizeof(char));
	char* pom = calloc(10, sizeof(char));
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
	
	while(1){
		uint32_t knobs = getKnobsValue(mem_base);
		unsigned char* val = numToCharRGB(knobs);
		sprintf(pom, "%d-%d-%d", (int)val[0], (int)val[1], (int)val[2]);
		
		writeText(img, 20, line*20, pom);
		repaintScreen(lcd_base, img);
		line++;
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
	free(address);
	free(img);
	return 0;
}



