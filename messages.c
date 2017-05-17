#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "messages.h"

#define ALC 0x0000001
#define PROTOCOL 0x0001000
#define TYPE 0
uint32_t charToNumRGB(unsigned char* RGB){
	uint32_t rgb = 0;
	uint32_t mask = 0;
	
	mask = RGB[2];
	rgb = (rgb|mask);
	
	mask = RGB[1];
	mask = mask<<8;
	rgb = (rgb|mask);
	
	mask = RGB[0];
	mask = mask<<16;
	rgb = (rgb|mask);
	
	return rgb;
}

unsigned char* numToCharRGB(uint32_t RGB){
	uint32_t mask = 255;
	unsigned char* rgb = calloc(sizeof(char), 3);

	rgb[2] = (mask&RGB);

	mask = mask<<8;
	mask = (mask&RGB)>>8;
	rgb[1] = mask;
	
	mask = 255;
	mask = mask<<16;
	mask = (mask&RGB)>>16;
	rgb[0] = mask;
	
	return rgb;
}


InfoMessage* createInfoMessage(unsigned char* wallRGB, unsigned char* ceilingRGB, char* text, char* image){
	InfoMessage* info = malloc(sizeof(InfoMessage));
	
	info->ceilingRGB = charToNumRGB(ceilingRGB);
	info->wallsRGB = charToNumRGB(wallRGB);
	
	if(text==NULL)text="This is [info].";
	memcpy(info->text, text, 16);
	
	if(image==NULL) image = calloc(512, sizeof(char));
	memcpy(info->text, text, 512);
	
	return info;
}

MessageHead* getMessageHead(int type){
	MessageHead* head = malloc(sizeof(MessageHead));
	head->ALC1 = ALC;
	head->protocol = PROTOCOL;
	head->type = (uint32_t) type;
	return head;
}



char* printMessage(MessageHead* head, EditMessage* message){
	printf("Received walls: %d %d %d\n", message->wallsRGB[0], message->wallsRGB[1], message->wallsRGB[2]);
	int16_t w[3];
	int16_t c[3];
	
	for(int i = 0;i<3;i++){
		w[i] = (unsigned int)message->wallsRGB[i];
		c[i] = (unsigned int)message->ceilingRGB[i];
	}
	
	char* str = calloc(sizeof(char), 80); 
	sprintf(str, "ALC: %d\nPROTOCOL: %d\nTYPE: %d\nRGB_WALL: %d %d %d\nRGB_CEILING: %d %d %d"
				, head->ALC1, head->protocol,
				head->type, w[0], w[1], w[2], 
				c[0], c[1], c[2]);
	return str;
}

