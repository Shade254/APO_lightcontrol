#ifndef IMGWRITE_H_
#define IMGWRITE_H_

int initCommunication();

void* receiveBytes(int SOCKET, int bytesNum, char* address);

int sendBytes(int SOCKET, void* bytes, char* ip, int length);

int broadcast(int SOCKET, void* bytes, int length);

#endif
