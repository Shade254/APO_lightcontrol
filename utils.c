#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
int getIndexIncrement(int lastVal, int curVal){
	if(lastVal != curVal){
		if(abs(curVal-lastVal) > 250){
			if(curVal>lastVal) return -1;
			if(lastVal>curVal) return +1;
		} else{
			if(curVal>lastVal) return +1;
			if(lastVal>curVal) return -1;
		}
	}
	return 0;
}


uint8_t* getValueIncrement(uint32_t old, uint32_t new){
	uint8_t or, og, ob;
	uint8_t nr, ng, nb;
	
	uint8_t* increment = calloc(3, sizeof(uint8_t));
	
	or = (old & 0x00FF0000)>>18;
	og = (old & 0x0000FF00)>>10;
	ob = (old & 0x000000FF)>>2;
	
	nr = (new & 0x00FF0000)>>18;
	ng = (new & 0x0000FF00)>>10;
	nb = (new & 0x000000FF)>>2;
	
	or += (or-nr);
	og += (og-ng);
	ob += (ob-nb);
	
	increment[0] = or;
	increment[1] = og;
	increment[2] = ob;
	
	printf("%d-%d-%d\n", or, og, ob);
	
	return increment;
}


int* getUnique(char** ips, int length){
	int* unique = calloc(length, sizeof(int));
	int* returnVal = calloc(length, sizeof(int));
	int n;
	int j, i;
	n = 0;
	
	for (i = 0; i < length; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			if (!strcmp(ips[i], ips[unique[j]]))
				break;
		}

		if (j == n)
			unique[n++] = i;
	}
			
	for(int i = 0; i < length;i++){
		returnVal[unique[i]] = 1;
	}		
	
	return returnVal;
}

uint32_t charToNumRGB(unsigned char* RGB){
	uint32_t rgb = 0;
	uint32_t mask = 0;
	
	mask = RGB[2];
	rgb = (rgb|mask);
	
	mask = RGB[1];
	mask = mask<<8;
	rgb = (rgb|mask);
	
	mask = RGB[0];
	mask = mask<<16;
	rgb = (rgb|mask);
	
	return rgb;
}

unsigned char* numToCharRGB(uint32_t RGB){
	uint32_t mask = 255;
	unsigned char* rgb = calloc(sizeof(char), 3);

	rgb[2] = (mask&RGB);

	mask = mask<<8;
	mask = (mask&RGB)>>8;
	rgb[1] = mask;
	
	mask = 255;
	mask = mask<<16;
	mask = (mask&RGB)>>16;
	rgb[0] = mask;
	
	return rgb;
}
