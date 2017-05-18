#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "messages.h"

#define ALC 0x0000001
#define PROTOCOL 0x0001000
#define TYPE 0

InfoMessage* createInfoMessage(unsigned char* wallRGB, unsigned char* ceilingRGB, char* text, int16_t* image){
	InfoMessage* info = malloc(sizeof(InfoMessage));
	
	info->ceilingRGB = charToNumRGB(ceilingRGB);
	info->wallsRGB = charToNumRGB(wallRGB);
	
	if(text==NULL)text="This is [info].";
	memcpy(info->text, text, 16);
	
	if(image==NULL) image = calloc(512, sizeof(char));
	memcpy(info->image, image, 512);
	
	return info;
}
EditMessage* createEditMessage(unsigned char* walls, unsigned char* ceiling){
	EditMessage* m = malloc(sizeof(EditMessage));
	for(int i = 0;i<3;i++){
		m->wallsRGB[i] = (int16_t)walls[i];
		m->ceilingRGB[i] = (int16_t)ceiling[i];
	}
	return m;
}

MessageHead* getMessageHead(int type){
	MessageHead* head = malloc(sizeof(MessageHead));
	head->ALC1 = ALC;
	head->protocol = PROTOCOL;
	head->type = (uint32_t) type;
	return head;
}

char* printInfoMessage(MessageHead* head, InfoMessage* message){
	char* str = calloc(sizeof(char), 80); 
	char* w = numToCharRGB(message->wallsRGB);
	char* c = numToCharRGB(message->ceilingRGB);
	printf(str, "ALC: %d\nPROTOCOL: %d\nTYPE: %d\nRGB_WALL: %d (%c-%c-%c)\nRGB_CEILING: %d (%c-%c-%c)\nTEXT: %s\n\n\nIMAGE: %s"
				, head->ALC1, head->protocol,
				head->type, message->wallsRGB, w[0],w[1], w[2], message->ceilingRGB,
				c[0], c[1], c[2], message->text, (char*)message->image);
	sprintf(str, "ALC: %d\nPROTOCOL: %d\nTYPE: %d\nRGB_WALL: %d (%c-%c-%c)\nRGB_CEILING: %d (%c-%c-%c)\nTEXT: %s\n\n\nIMAGE: %s"
				, head->ALC1, head->protocol,
				head->type, message->wallsRGB, w[0],w[1], w[2], message->ceilingRGB,
				c[0], c[1], c[2], message->text, (char*)message->image);
	return str;	
}

char* printEditMessage(MessageHead* head, EditMessage* message){
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

