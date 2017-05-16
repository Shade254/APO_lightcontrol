#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imgwrite.h"
#include "font_types.h"
#include "netcom.h"
#include "messages.h"


#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255

int main(){
	int socket = initCommunication();
	char* address = malloc(16*sizeof(char));
		
	if(socket != 0){
		printf("Socket [OK]\n");
	} else{
		printf("Socket [ERROR]");
		exit(1);
	}
	
	unsigned char rgbC[] = {255, 255, 255};
	unsigned char rgbW[] = {255, 255, 255};
	char* ip = "127.0.0.1";
	
	InfoMessage* message = createInfoMessage(rgbW, rgbC, NULL,NULL);
	printf("Sending mesage\n");
	printf("ALC %d\n", message->ALC1);
	printf("protocol %d\n", message->protocol);
	printf("type %d\n", message->type);
	printf("ceiling %d\n", message->ceilingRGB);
	printf("walls %d\n", message->wallsRGB);
	printf(" Text %s\n", message->text);
	printf("Image %s\n", message->image);
	
	void* data = (void*)message;
	broadcast(socket, data, sizeof(*message));	

	free(address);
	free(message);
	return 0;
}



