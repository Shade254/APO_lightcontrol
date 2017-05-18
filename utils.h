#ifndef UTILS_H_
#define UTILS_H_

uint8_t* getValueIncrement(uint32_t old, uint32_t new);

int getIndexIncrement(int lastVal, int curVal);

int* getUnique(char** ips, int length);

uint32_t charToNumRGB(unsigned char* RGB);

unsigned char* numToCharRGB(uint32_t RGB);

#endif
