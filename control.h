#ifndef CONTROL_H_
#define CONTROl_H_

unsigned char *initMemBase();

uint32_t getKnobsValue(unsigned char* mem_base);

int* getButtonValue(uint32_t knobs);

#endif
