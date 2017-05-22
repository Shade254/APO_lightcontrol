#include "netcom.h"
#include <inttypes.h>
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
	free(head);

	if(r) return 1;
	else return 0;
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

	if(r) return 1;
	else return 0;	
}


AreaInfo* getBroadcasters(int socket, int numOfMessages){
	InfoMessage** incomMes = calloc(numOfMessages, sizeof(InfoMessage*));
	MessageHead** incomHead = calloc(numOfMessages, sizeof(MessageHead*));
	
	char** ips = calloc(numOfMessages, sizeof(char*));
	
	printf("Getting...");
	
	for(int i = 0;i<10;i++){
			char* address = calloc(16, sizeof(char));
			void* bytes = receiveBytes(socket, sizeof(MessageHead) + sizeof(InfoMessage), address);
			printf("Adress: %s\n", address);
	
			MessageHead* head = (MessageHead*) bytes;
			InfoMessage* message = (InfoMessage*)(bytes+sizeof(MessageHead));
			printInfoMessage(head, message);
			
			ips[i] = address;
			incomMes[i] = message;
			incomHead[i] = head;
	}
	
	for(int i = 0;i<numOfMessages;i++){
		printf("IP(%d): %s\n",i, ips[i]);
	}
	
	int *uniq = getUnique(ips, numOfMessages);
	int sum = 0;
	for(int i = 0;i<numOfMessages;i++){
			printf("%d\n", uniq[i]);
			if(uniq[i]){
				sum++;
			}
	}
	
	
	printf("-------------------------");
	AreaInfo* area = calloc(1, sizeof(AreaInfo));
	area->size = sum;
	area->ips = calloc(sum, sizeof(char*));
	area->heads = calloc(sum, sizeof(MessageHead*));
	area->messages = calloc(sum, sizeof(InfoMessage*));
	
	int counter = 0; 
	
	
	for(int i = 0; i < numOfMessages;i++){		
		if(uniq[i]){
			area->ips[counter] = ips[i];
			area->heads[counter] = incomMes[i];
			area->messages[counter] = incomHead[i];
			printf("From address %s:\n", area->ips[counter]);
			printInfoMessage(area->heads[counter], area->messages[counter]);
			counter++;
		} else {
			free(ips[counter]);
			free(incomMes[counter]);
			free(incomHead[counter]);
		}
	}
	
	free(ips);
	free(incomMes);
	free(incomHead);
	return area;
}
