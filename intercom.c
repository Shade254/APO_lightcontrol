#include "netcom.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "messages.h"
#include "intercom.h"

int broadcastInfo(int socket, unsigned char* walls, unsigned char* ceiling, char* text, int16_t* image){
	MessageHead* head = createMessageHead(0);
	InfoMessage* message = createInfoMessage(walls, ceiling, text, image);
	void* data = calloc(1, sizeof(MessageHead) + sizeof(InfoMessage));
	memcpy(data, head, 12);
	void* pom = ((MessageHead*)data + 1);
	memcpy(pom, message, 536);
	
	printInfoMessage((MessageHead*)data, (InfoMessage*)pom);

	int r = broadcast(socket, data, sizeof(MessageHead) + sizeof(InfoMessage));
	
	free(data);
	free(message);

	return r;
}

int sendEdit(int socket, unsigned char* walls, unsigned char* ceiling, char* ip){
	MessageHead* head = createMessageHead(0);
	EditMessage* message = createEditMessage(walls, ceiling);
	void* data = calloc(1, sizeof(MessageHead) + sizeof(EditMessage));
	memcpy(data, head, 12);
	void* pom = ((MessageHead*)data + 1);
	memcpy(pom, message, sizeof(EditMessage));
	
	printEditMessage((MessageHead*)data, (EditMessage*)pom);

	int r = sendBytes(socket, data, ip,sizeof(MessageHead) + sizeof(InfoMessage));
	
	free(data);
	free(message);
	free(head);

	return r;
}


AreaInfo* getBroadcasters(int socket, int numOfMessages){
	InfoMessage** incomMes = calloc(numOfMessages, sizeof(InfoMessage*));
	MessageHead** incomHead = calloc(numOfMessages, sizeof(MessageHead*));
	
	
	char** ips = calloc(numOfMessages, sizeof(char*));
	
	
	printf("Getting...\n");
	for(int i = 0;i<numOfMessages;i++){
			ips[i] = calloc(16, sizeof(char));
			void* bytes = receiveBytes(socket, sizeof(MessageHead)+sizeof(InfoMessage), ips[i]);
			MessageHead* head = (MessageHead*) bytes;
			void* pom = (void*)(head + 1);
			InfoMessage* message = (InfoMessage*)pom;
			printf("From address %s:\n", ips[i]);
			printInfoMessage(head, message);
			incomMes[i] = message;
			incomHead[i] = head;
	}
	
	char** labels = calloc(numOfMessages, sizeof(char*));
	for(int i = 0;i<numOfMessages;i++){
		labels[i] = incomMes[i]->text;
	}
	
	
	int *uniq = getUnique(labels, numOfMessages);
	int sum = 0;
	for(int i = 0;i<numOfMessages;i++){
		printf("\n%d\n", uniq[i]);
			if(uniq[i]){
				sum++;
			}
	}
	
	printf("------------I got %d unique messages-------------\n", sum);
	
	AreaInfo* area = calloc(1, sizeof(AreaInfo));
	area->size = sum;
	area->ips = calloc(sum, sizeof(char*));
	area->heads = calloc(sum, sizeof(MessageHead*));
	area->messages = calloc(sum, sizeof(InfoMessage*));
	
	int counter = 0; 
	
	
	for(int i = 0; i < numOfMessages;i++){		
		if(uniq[i]){
			area->ips[counter] = ips[i];
			area->heads[counter] = incomHead[i];
			area->messages[counter] = incomMes[i];
			
			printf("From address %s:\n", area->ips[counter]);
			printInfoMessage(area->heads[counter], area->messages[counter]);
			counter++;
		} else {
			free(incomHead[i]);
		}
	}
	
	return area;
}
