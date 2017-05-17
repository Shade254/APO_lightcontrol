#include <stdlib.h>
#include <stdio.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

unsigned char* initMemBase(){
	unsigned char* base = map_phys_address(SPILLED_REG_BASE_PHYS,
	SPILED_REG_SIZE, 0);
	
	if(base == NULL){
		printf("[ERROR] Knobs init\n");
		return NULL;
	}
	
	
	return base;
}

uint32_t getKnobsValue(unsigned char* mem_base){
	return *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
}


