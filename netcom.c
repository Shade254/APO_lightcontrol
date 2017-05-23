#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "netcom.h"

#define SEND_PORT 55555
#define BIND_PORT 55555

int initCommunication(){
	struct sockaddr_in bindaddr;
	
		
	int sockfd = 0;

   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        return 0;
    }
	
	
	memset(&bindaddr, 0, sizeof(bindaddr));
	bindaddr.sin_family = AF_INET;
	bindaddr.sin_port = htons(BIND_PORT);
	bindaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1) {
		perror("bind");
		return 0;
	}
	
	int yes=1;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt (SO_REUSEADDR)");
		return 0;
	}
	
	int broadcast = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        return 0;
    }
    
	return sockfd;
}

int sendBytes(int SOCKET, void* bytes, char* ip, int length){
	struct sockaddr_in receiverInfo;
	
	receiverInfo.sin_family = AF_INET;
	receiverInfo.sin_port = htons(SEND_PORT);
	receiverInfo.sin_addr.s_addr = inet_addr(ip);
	
	if (sendto(SOCKET, bytes, length, 0, (struct sockaddr*)&receiverInfo, sizeof(receiverInfo)) == -1){
		perror("not send: ");
		return 0;
	}
	
	return 1;
}

int broadcast(int SOCKET, void* bytes, int length){
    struct sockaddr_in config;

    if(SOCKET < 0)
        return 0;

    memset(&config, '\0', sizeof(struct sockaddr_in));
    config.sin_family = AF_INET;
    config.sin_port = (in_port_t)htons(SEND_PORT);
    config.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if(sendto(SOCKET, bytes, length, 0, (struct sockaddr *)&config, sizeof(struct sockaddr_in)) < 0){
       return 0;
	}
	
   return 1;
}


void* receiveBytes(int SOCKET, int length, char* address){
	
	struct sockaddr_storage sender;
	
	void* buffer = malloc(length * sizeof(char));
		
	unsigned int fromlen = sizeof(sender);
	
	int sum = 0;
		while(sum < length){
			sum += recvfrom(SOCKET, buffer, length-sum, 0, (struct sockaddr*)&sender, &fromlen);
			printf("%d/%d received\n", sum, length);
		}
	inet_ntop(sender.ss_family, &(((struct sockaddr_in *)&sender)->sin_addr),address, INET_ADDRSTRLEN);
	
	return buffer;
}






