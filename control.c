#include <stdlib.h>
#include <stdio.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

unsigned char* initMemBase(){
	unsigned char* base = map_phys_address(SPILED_REG_BASE_PHYS,
	SPILED_REG_SIZE, 0);
	
	if(base == NULL){
		printf("[ERROR] Knobs init\n");
		return NULL;
	}
	
	
	return base;
}

int* getButtonValue(uint32_t knobs){
	int* result = calloc(3, sizeof(int));
	
	uint32_t mask = 255;
	mask = mask<<24;
	mask = (mask&knobs)>>24;
	
	char value = mask;
	result[2] = (value&1);
	result[1] = (value&(1<<1))>>1;
	result[0] = (value&(1<<2))>>2;
	
	return result;
}

char* getValueIncrement(uint32_t old, uint32_t new){
	uint8_t or, og, ob;
	uint8_t nr, ng, nb;
	
	char* increment = calloc(3, sizeof(char));
	
	or = (old & 0x00FF0000)>>18;
	og = (old & 0x0000FF00)>>10;
	ob = (old & 0x000000FF)>>2;
	
	nr = (new & 0x00FF0000)>>18;
	ng = (new & 0x0000FF00)>>10;
	nb = (new & 0x000000FF)>>2;
	
	increment[0] = (nr-or);
	increment[1] = (ng-og);
	increment[2] = (nb-ob);
	
	printf("%d-%d-%d\n", increment[0], increment[1], increment[2]);
	
	return increment;
}

uint32_t getKnobsValue(unsigned char* mem_base){
	return *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
}


