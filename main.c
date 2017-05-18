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
#include "utils.h"


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

void sendEdit(unsigned char* walls, unsigned char* ceiling, char* text, int socket, char* ip){
	MessageHead* head = getMessageHead(SET_TYPE);
	EditMessage* message = createEditMessage(walls, ceiling);
	
	int h = sendBytes(socket, (void*)head, ip, sizeof(MessageHead));
	int b = sendBytes(socket, (void*)message, ip, sizeof(EditMessage));
	
	if(h&&b) printf("[OK] Sent\n");
	else printf("[ERROR] Not Sent\n");
	char* mes = printEditMessage(head, message);
	printf("%s\n", mes);
	
}

void runSettings(char* ip, char* text, unsigned char* mem_base, unsigned char* lcd_base, int socket){
	char* pom = calloc(60, sizeof(char));
	char* title = calloc(50, sizeof(char));
	sprintf(title, "Settings of %s\n on adress %s", text, ip);
	char arrow[2] = {16,0};
	
	unsigned char wallRGB[3] = {0, 0, 0};
	unsigned char ceilingRGB[3] = {0, 0, 0};
	int flag = 0;
	unsigned char* oldValues;
	uint32_t old = getKnobsValue(mem_base);
	oldValues = numToCharRGB(old);
	
	uint32_t new;
	
	while(1){
		new = getKnobsValue(mem_base);
		
		int* b = getButtonValue(new);
		if(b[0]) break;
		if(b[1]) flag = !flag;
		if(b[2]){
			 sendEdit(wallRGB, ceilingRGB, text, socket, ip);
			 break;
		 }
		 
		
		unsigned char* newValues = numToCharRGB(new);
		
		for(int i = 0;i<3;i++){
			if(flag){
				wallRGB[i] += getIndexIncrement(oldValues[i], newValues[i]);
			}
			else{
				ceilingRGB[i] += getIndexIncrement(oldValues[i], newValues[i]);
			}
		}
		
		sprintf(pom, "Walls - R:%d G:%d B:%d", wallRGB[0], wallRGB[1], wallRGB[2]);
		Image* img = createTextScreen(20, 20, title);
		writeText(img, 20, 60, pom);
		sprintf(pom, "Ceiling - R:%d G:%d B:%d", ceilingRGB[0], ceilingRGB[1], ceilingRGB[2]);
		writeText(img, 20, 80, pom);
		
		if(flag){
			writeText(img, 0, 60, arrow);
		} else {
			writeText(img, 0, 80, arrow);
		}
		
		repaintScreen(lcd_base, img);
		free(img);
		free(oldValues);
		oldValues = newValues;
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
}

InfoMessage* getBroadcasters(int socket, int numOfMessages){
	int length = 0;
	InfoMessage** incoming = calloc(numOfMessages, sizeof(InfoMessage*));
	char** ips = calloc(numOfMessages, sizeof(char*));
	void* cur = (void*)incoming;
	
	printf("Getting...");
	
	for(int i = 0;i<10;i++){
			char* address = calloc(16, sizeof(char));
			void* bytes = receiveBytes(socket, sizeof(MessageHead) + sizeof(InfoMessage), address);
			printf("Adress: %s\n", address);
	
			MessageHead* head = (MessageHead*) bytes;
			InfoMessage* message = (InfoMessage*)(bytes+sizeof(MessageHead));
			printf("%s\n", printInfoMessage(head, message));
			
			ips[i] = address;
			incoming[i] = message;
	}
	
	for(int i = 0;i<numOfMessages;i++){
		printf("%s\n", ips[i]);
	}
	
	int *uniq = getUnique(ips, numOfMessages);
	int sum = 0;
	
	for(int i = 0;i<numOfMessages;i++){
			printf("%d\n", uniq[i]);
			if(uniq[i]){
				sum++;
			}
	}
	
	char** response = calloc(sum, sizeof(InfoMessage*));
	cur = (void*) response; 
	for(int i = 0; i < numOfMessages;i++){
		if(uniq[i]){
			cur = ips[i];
			cur += sizeof(char*);
		}
	}
	
	for(int i = 0; i < sum;i++){
		printf("%s\n", response[i]);
	}
	
	free(incoming);
	return response;
}

void broadcastMe(int socket){
	MessageHead* head = getMessageHead(0);
	InfoMessage* message = createInfoMessage(thisWalls, thisCeiling, thisText, thisImage);
	printInfoMessage(head, message);
	int h = broadcast(socket, (void*)head, sizeof(MessageHead));
	int b = broadcast(socket, (void*)message, sizeof(InfoMessage));
	if(h&b) printf("\n[OK] broadcasted sucessfully\n\n\n");
	else printf("[ERROR] Not broadcasted\n");
	free(head);
	free(message);
}

int init(){
	int socket = initCommunication();
	if(socket == 0) return 0;
	lcd_base = initScreen();
	mem_base = initMemBase();
	if(lcd_base == NULL || mem_base == NULL) return 0;
	thisCeiling = calloc(3, sizeof(char));
	thisWalls = calloc(3, sizeof(char));
	thisText = "Ahoj, ja Borat!";
	
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
	int socket = init();
	if(socket == 0){
		printf("[ERROR] Init failed\n");
		exit(1);
	} else{
		printf("[OK] Init\n");
	}
	
	unsigned long milisLast;
	
	char* pom = calloc(30, sizeof(char));
	
	const char* a[5];
	a[0] = "ahoj";
	a[1] = "lidi";
	a[2] = "ahoj";
	a[3] = "ahoj";
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
	free(val);
	milisLast = time(NULL);
	
	while(1){
		uint32_t knobs = getKnobsValue(mem_base);
		int* buttons = getButtonValue(knobs);
		if(buttons[1]){
			runSettings("127.0.0.1", a[index], mem_base, lcd_base, socket);
		} 
		
		if(buttons[0]){
			exit(1);
		}
		
		val = numToCharRGB(getKnobsValue(mem_base));
		printf("%d %d %d\n", val[0], val[1], val[2]);
		index += getIndexIncrement(lastVal, (int)val[0]);
		
		if(index<0) index = (5+index);
		index = index%5;
		printf("index = %d\n", index);
		lastVal = (int)val[0];
		img = createMenuScreen(a, 5, index);
		repaintScreen(lcd_base, img);
		free(img);
		free(val);
		
		if((time(NULL) - milisLast) >= 1){
			printf("Broadcasting...\n");
			broadcastMe(socket);
			milisLast = time(NULL);
			printf("EOL\n");
		}
		
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
	}
	
	return 0;
}



