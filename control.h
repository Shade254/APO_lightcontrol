#ifndef CONTROL_H_
#define CONTROl_H_

unsigned char *initMemBase();

uint32_t getKnobsValue(unsigned char* mem_base);

int* getButtonValue(uint32_t knobs);

char* getValueIncrement(uint32_t old, uint32_t new);

#endif
