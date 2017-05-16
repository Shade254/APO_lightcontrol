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
	info->ALC1 = (uint32_t)ALC;
	info->protocol = (uint32_t)PROTOCOL;
	info->type = (uint32_t)TYPE;
	
	info->ceilingRGB = charToNumRGB(ceilingRGB);
	info->wallsRGB = charToNumRGB(wallRGB);
	
	if(text==NULL)text="This is [info].";
	memcpy(info->text, text, 16);
	
	if(image==NULL) image = calloc(512, sizeof(char));
	memcpy(info->text, text, 512);
	
	return info;
}

char* printMessage(InfoMessage* message){
	char* str = calloc(sizeof(char), 80); 
	sprintf(str, "%d-%d-%d-%d-%d-%s", message->ALC1, message->protocol, 
	message->type, message->ceilingRGB, message->wallsRGB, message->text);
	return str;
}

