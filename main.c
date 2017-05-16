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
	Image* img;
	int line = 0;
	
	if(socket != 0){
		img = createTextScreen(20, line*20, "[INFO] Socket is valid");
	} else{
		img = createTextScreen(20, line*20, "[ERROR] Inicialization failed");
		exit(1);
	}
	showScreen("1.ppm", img);
	line++;
	
	void* data = receiveBytes(socket, 548, address);
	InfoMessage* message = (InfoMessage*)data;
	printf("GOT message\n");
	printf("ALC %d\n", message->ALC1);
	printf("protocol %d\n", message->protocol);
	printf("type %d\n", message->type);
	printf("ceiling %d\n", message->ceilingRGB);
	printf("walls %d\n", message->wallsRGB);
	printf(" Text %s\n", message->text);
	printf("Image %s\n", message->image);
	
		
	free(address);
	free(img);
	return 0;
}



