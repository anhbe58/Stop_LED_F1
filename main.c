#include "ch552.h"
#include <stdint.h>
void delay100ms(void);
const unsigned int led_array[6][20] = {
	{100,1000, 100,1000, 100,1000, 100,1000, 100,1000, 0,1000, 0,1000, 0,1000, 0,1000, 0,1000},
	{100,500, 100,500, 100,500, 100,500, 0,2000, 100,500, 100,500, 100,500, 0,1000, 0,1000},
	{100,500, 100,500, 100,500, 4000,500, 100,500, 100,500, 100,500, 4000,0, 0,0, 0,500},
	{100,500, 100,500, 4000,500, 100,500, 100,500, 4000,500, 100,500, 100,500, 0,0, 4000,500},
	{100,100, 100,100, 100,100, 500,500, 500,500, 0,1000, 0,0, 0,0, 0,0, 0,0},
	{30000,5000, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0}
};

void delay100ms(void){
	uint32_t i;
	for(i = 0;i < 0x003FF; i++){
	}
}
unsigned char led_time_index = 0, led_mode = 5, button_count = 0;
volatile unsigned int led = 0;
void Timer2_ISR(void) __interrupt (5)
{
	led++;
	if(led > led_array[led_mode][led_time_index]) {	
		led = 0;
		led_time_index++;
		if(led_array[led_mode][led_time_index] > 0) P1 ^= (1 << 5);
			else led_time_index++;
		if(led_time_index > 19) led_time_index = 0;
		
	}
}
void main(){
	/* Fsys = 750kHz */
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG = 0x81;
	IE |= (1 << 5);
	IE |= (1 << 7);
	SAFE_MOD = 0x00;
	

	//T2MOD |= (1 << 6);
	//T2MOD |= (1 << 7);
	T2MOD &= ~(1 << 6);
	
	T2CON &= ~(1 << 1);
	
	
	//RCAP2L = 0xF8;
	//RCAP2H = 0xFF;
	T2CON |= (1 << 2);
	
	/* P1.5 */
	/* Push-pull */
	P1_MOD_OC &= ~(1 << 5);
	P1_DIR_PU |= (1 << 5);
	
	/* P3.0 */
	/* Input-only */
	P3_MOD_OC |= (1 << 0);
	P3_DIR_PU &= ~(1 << 0);
	
	P1 |= (1 << 5);
	uint8_t trigger_pin = 0;
	while(1){
		//if(TF2 == 1){			
			
		//}
		// if((P3 >> 0) & 1){
			// delay100ms();
			// if((P3 >> 0) & 1){
				// P1 ^= (1 << 5);
			// }
			// while((P3 >> 0) & 1){
				// P1 ^= (1 << 5);
				// delay100ms();
			// }
		// }
		//P1 |= (1 << 5);
		
		// delay100ms();
	}		
}
