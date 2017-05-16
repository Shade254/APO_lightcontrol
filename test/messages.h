#ifndef MESSAGES_H_ 
#define MESSAGES_H_

typedef struct {
	uint32_t ALC1;
	uint32_t protocol;
	uint32_t type;
	uint32_t ceilingRGB;
	uint32_t wallsRGB;
	char text[16];
	char image[512];
} InfoMessage;

InfoMessage* createInfoMessage(unsigned char* wallRGB, unsigned char* ceilingRGB, char* text, char* image);

char* printMessage(InfoMessage* message);

#endif
