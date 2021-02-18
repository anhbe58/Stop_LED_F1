#include "ch552.h"
#include <stdint.h>
union {
	struct {
		unsigned char default_led: 4;
		unsigned char stop_led: 4;
	};
	unsigned char data_epprom;
} LED;
void delay100ms(void);
const unsigned int led_array[6][20] = {
	{100,1000, 100,1000, 100,1000, 100,1000, 100,1000, 0,1000, 0,1000, 0,1000, 0,1000, 0,1000},
	{100,100, 100,100, 100,100, 500,500, 500,500, 0,1000, 0,0, 0,0, 0,0, 0,0},
	{100,500, 100,500, 100,500, 4000,500, 100,500, 100,500, 100,500, 4000,0, 0,0, 0,500},
	{100,500, 100,500, 100,500, 100,500, 0,2000, 100,500, 100,500, 100,500, 0,1000, 0,1000},
	{100,500, 100,500, 4000,500, 100,500, 100,500, 4000,500, 100,500, 100,500, 0,0, 4000,500},	
	{1000,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0}
};

void delay100ms(void){
	uint32_t i;
	for(i = 0;i < 0x0000F; i++){
	} 
}
unsigned char led_time_index = 0, led_mode = 4, button_count = 0, stop_led = 0;
unsigned long timer = 0, timer_capture = 0;
volatile unsigned int led = 0;
void Timer2_ISR(void) __interrupt (5)
{
	led++;
	timer++;
	if(led > led_array[led_mode][led_time_index]) {	
		led = 0;
		led_time_index++;
		if(led_array[led_mode][led_time_index] > 0) P1 ^= (1 << 5);
			else led_time_index++;
		if(led_time_index > 19) led_time_index = 0;
		
	}
}
void write_eeprom(unsigned char data){
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG |= bDATA_WE;
	ROM_ADDR = 0xC000;
	ROM_DATA_L = data;
	ROM_CTRL = ROM_CMD_WRITE;
	SAFE_MOD = 0x00;
	GLOBAL_CFG &= ~bDATA_WE;
}
unsigned char read_eeprom(){
	ROM_ADDR = 0xC000;
	ROM_CTRL = ROM_CMD_READ;
	return (unsigned char) ROM_DATA_L;
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
	LED.data_epprom = read_eeprom();
	led_mode = LED.default_led;
	if(led_mode > 5) {
		led_mode = 0; write_eeprom(0);
	}
	while(1){
		if((P3 >> 0) & 1){
			delay100ms();
			if((P3 >> 0) & 1){
				if(button_count == 0) timer_capture = timer;
				button_count++;				
			}
			while((P3 >> 0) & 1){
				// P1 ^= (1 << 5);
				// delay100ms();
				led_mode = LED.stop_led;
			}	
			led_mode = LED.default_led;			
		}
		if((timer - timer_capture > 25000) && (button_count > 0)) {
			//led_mode++;
			//if(led_mode > 5) led_mode = 0;
			switch(button_count){				
				case 5:
					LED.default_led++;
					if(LED.default_led > 4) LED.default_led = 0;
					write_eeprom(LED.data_epprom);
					led_mode = LED.default_led;
					button_count = 0;
					break;
				case 7:
					LED.stop_led++;					
					if(LED.stop_led > 5) LED.stop_led = 0;
					write_eeprom(LED.data_epprom);					
					button_count = 0;
					break;
				default:
					button_count = 0;
					break;
			}
		}
		//P1 |= (1 << 5);
		
		// delay100ms();
	}		
}
