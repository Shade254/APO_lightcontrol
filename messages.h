#ifndef MESSAGES_H_ 
#define MESSAGES_H_

typedef struct {
	uint32_t ceilingRGB;
	uint32_t wallsRGB;
	char text[16];
	char image[512];
} InfoMessage;

typedef struct {
	uint32_t ALC1;
	uint32_t protocol;
	uint32_t type;
} MessageHead;

typedef struct {
	int16_t wallsRGB[3];
	int16_t ceilingRGB[3];
} EditMessage;

InfoMessage* createInfoMessage(unsigned char* wallRGB, unsigned char* ceilingRGB, char* text, char* image);

char* printMessage(MessageHead* head, EditMessage* message);

MessageHead* getHead(int type);

uint32_t charToNumRGB(unsigned char* RGB);

unsigned char* numToCharRGB(uint32_t RGB);
#endif
