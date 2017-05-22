#include <stdlib.h>
#include <stdio.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "imgwrite.h"

unsigned char* initScreen(){
	unsigned char* lcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS,
	PARLCD_REG_SIZE, 0);
	
	if(lcd_mem_base == NULL){
		printf("[ERROR] in init Screen");
		return NULL;
	}
	
	parlcd_hx8357_init(lcd_mem_base);
	parlcd_write_cmd(lcd_mem_base, 0x2c);
	
	return lcd_mem_base;
}

int rgbTo565(unsigned char r, unsigned char g, unsigned char b){
	return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
}

void repaintScreen(unsigned char *lcd_base, Image* screen){
	parlcd_write_cmd(lcd_base, 0x2c);
	
	for(int i = 0;i<(screen->x * screen->y);i++){
		Pixel p = screen->data[i];
		int rgb = rgbTo565(p.red, p.green, p.blue);
		parlcd_write_data(lcd_base, rgb);
	}
}
