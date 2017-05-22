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


uint32_t getKnobsValue(unsigned char* mem_base){
	return *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
}


