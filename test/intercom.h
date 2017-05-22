#ifndef INTERCOM_H_
#define INTERCOM_H_
#include "messages.h"

typedef struct{
	MessageHead** heads;
	InfoMessage** messages;
	char** ips;
	int size;
}AreaInfo;

int broadcastInfo(int socket, unsigned char* walls, unsigned char* ceiling, char* text, int16_t* image);

int sendEdit(int socket, unsigned char* walls, unsigned char* ceiling, char* ip);

AreaInfo* getBroadcasters(int socket, int numOfMessages);

#endif
