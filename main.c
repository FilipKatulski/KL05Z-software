#include "spi.h"
#include "display.h"
#include "MKL05Z4.h"
uint8_t data = 10;

int main(void){
	uint32_t waittime = 0;
	
	//spiInitialize();
	displayInitialize();
	while(1){
		//sendData(data);
		
		move_XY(10, 10);
		print_character_2(0);
		//print_character_2(1);
		//print_character_2(2);
		
		print_character(0);
		//print_character(1);
		while(waittime != SystemCoreClock/500){
		waittime++;
		}
		waittime = 0;
		
	}
}