#include "MKL05Z4.h"
#include "display.h"
#include "spi.h"


uint32_t timer = 0;
void displayInitialize(void){
	
	
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
	
	PORTB->PCR[7] |= PORT_PCR_MUX(1); //dc
	PORTB->PCR[6] |= PORT_PCR_MUX(1); // RST
	PTB->PDDR |= dc_mask; //initialize GPIO as OUTPUT
	PTB->PDDR |= rst_mask; //initialize GPIO as OUTPUT
	PTB->PSOR |= dc_mask; //pull up
	
	
	PTB->PCOR |= rst_mask;
	
	while(timer != SystemCoreClock/500){
		timer++;
	}
	PTB->PSOR |= rst_mask;
	spiInitialize();
	/*
	// function set H = 0
	sendCommand(36); //0b00100100)
	// display blank
	sendCommand(8); //0b00001000
	// set y address
	sendCommand(64); //0b01000000
	// set x address
	sendCommand(128); //0b10000000
	// function set H = 1
	sendCommand(37); //0b00100101
	// temp coeff
	sendCommand(4); //0b00000100
	// bias system
	sendCommand(16); //0b00010000
	// set vop
	sendCommand(128); //0b10000000
	// poweron
	sendCommand(32);
	// display normal mode
	//sendCommand(9);
	*/
	
	sendCommand(FUNCTION_SET_MASK | H_MASK);
	sendCommand(BIAS_MASK | 3); // mux 1:48 0x13 // CONTRAST
	sendCommand(VOP_MASK | 0x36); // VOP 0xB6
	sendCommand(FUNCTION_SET_MASK);// basic mode 0x20
	sendCommand(DISPLAY_CONTROL_MASK | D_MASK); // normal mode 0x0C
	
	
}

// move pointer to XY location
void move_XY(uint8_t X,uint8_t Y)
{
   sendCommand(X_MASK | X);
   sendCommand(Y_MASK | Y);
}
;
// Table to decode char to lcd 5x8 font
const char font5x8[][5] = {
   {0x7e, 0x11, 0x11, 0x11, 0x7e}       /* 0x41 A */
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a}       /* 0x47 G */
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f}       /* 0x48 H */
}
;
// print char
void print_character(char x)
{
   uint8_t i;

   for(i=0;i<5;i++)
   {
        sendData(font5x8[x][i]);
   }
   sendData(0x00); // space between characters
}

void print_character_2(uint8_t x)
{
   uint8_t i;

   for(i=0;i<5;i++)
   {
        sendData(font5x8[x][i]);
   }
   sendData(0x00); // space between characters
}




void sendCommand(uint8_t cmd){
	PTB->PCOR |= dc_mask;
	spiWrite1(cmd);
}

void sendData(uint8_t data){
	PTB->PSOR |= dc_mask;
	spiWrite1(data);
}


