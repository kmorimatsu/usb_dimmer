#include "pico/stdlib.h"
#include "led8seg4.h"

unsigned int led8seg4_all;
volatile char led8seg4_on=0;
static struct repeating_timer g_led8seg_timer;
bool repeating_led8seg4_callback(struct repeating_timer *t) {
	static int led_pos=0;
	// Clear all anodes, first
	gpio_put(LED8SEG4_DIG1, 0);
	gpio_put(LED8SEG4_DIG2, 0);
	gpio_put(LED8SEG4_DIG3, 0);
	gpio_put(LED8SEG4_DIG4, 0);
	// Output to cathode
	switch(led_pos&7){
		case 0:
			gpio_put(LED8SEG4_DP, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_A, 0);
			break;
		case 1:
			gpio_put(LED8SEG4_A, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_B, 0);
			break;
		case 2:
			gpio_put(LED8SEG4_B, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_C, 0);
			break;
		case 3:
			gpio_put(LED8SEG4_C, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_D, 0);
			break;
		case 4:
			gpio_put(LED8SEG4_D, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_E, 0);
			break;
		case 5:
			gpio_put(LED8SEG4_E, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_F, 0);
			break;
		case 6:
			gpio_put(LED8SEG4_F, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_G, 0);
			break;
		default:
			gpio_put(LED8SEG4_G, 1);
			if (led8seg4_all&(1<<led_pos)) gpio_put(LED8SEG4_DP, 0);
			break;
	}
	// Output to anode
	if (led8seg4_on && (led8seg4_all&(1<<led_pos))) {
		switch(led_pos&0x18){
			case 0x00:
				gpio_put(LED8SEG4_DIG4, 1);
				break;
			case 0x08:
				gpio_put(LED8SEG4_DIG3, 1);
				break;
			case 0x10:
				gpio_put(LED8SEG4_DIG2, 1);
				break;
			default:
				gpio_put(LED8SEG4_DIG1, 1);
				break;
		}
	}
	// Increment
	led_pos=(led_pos+1)&0x1f;
	// All done
	return true;
}

static void init_pin(int p){
	gpio_init(p);
	gpio_set_dir(p, GPIO_OUT);
	gpio_put(p, 1);
	gpio_set_drive_strength(p, GPIO_DRIVE_STRENGTH_12MA);
}

static void init_common_pin(int p){
	gpio_init(p);
	gpio_set_dir(p, GPIO_OUT);
	gpio_put(p, 0);
	gpio_set_drive_strength(p, GPIO_DRIVE_STRENGTH_12MA);
}

void led8seg4_init(void){
	// Initialize GPIO for 8 segment LED array
	init_pin(LED8SEG4_A);
	init_pin(LED8SEG4_B);
	init_pin(LED8SEG4_C);
	init_pin(LED8SEG4_D);
	init_pin(LED8SEG4_E);
	init_pin(LED8SEG4_F);
	init_pin(LED8SEG4_G);
	init_pin(LED8SEG4_DP);
	init_common_pin(LED8SEG4_DIG1);
	init_common_pin(LED8SEG4_DIG2);
	init_common_pin(LED8SEG4_DIG3);
	init_common_pin(LED8SEG4_DIG4);
	
	// Initialize timer
	// In each call back, one LED is bright. There are 32 LEDs.
	// Call the function every 0.5 millisecond, resulting in 15.625 fps
	add_repeating_timer_us(-500, repeating_led8seg4_callback, NULL, &g_led8seg_timer);
	
	// Show 8888, first
	led8seg4_on=1;
	led8seg4_all=0x7f7f7f7f;
}

// Hex patterns from 0x0 to 0xF
const unsigned char led8seg4_hex2raw[]={
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111,
	0b01110111,
	0b01111100,
	0b01011000,
	0b01011110,
	0b01111001,
	0b01110001
};

void setPoint(int pos){
	int i;
	for(i=3;0<i;i--){
		if (i<=pos) break;
		// If showing non-zero, then break
		if (led8seg4_each[i]!=led8seg4_hex2raw[0]) break;
		// Clear zero character
		led8seg4_each[i]=0;
	}
	if (0<=pos && pos<=3) led8seg4_each[pos]|=0x80;
}

void show4digits(int num){
	int i;
	for(i=0;i<4;i++){
		led8seg4_each[i]=led8seg4_hex2raw[num%10];
		num/=10;
	}
}

void show4hex(int num){
	int i;
	for(i=0;i<4;i++){
		led8seg4_each[i]=led8seg4_hex2raw[num&0x0f];
		num>>=4;
	}
}

void hexNumber(int num){
	led8seg4_on=0;
	show4hex(num);
	setPoint(-1);
	led8seg4_on=1;
}

void integerNumber(int num){
	led8seg4_on=0;
	show4digits(num);
	setPoint(-1);
	led8seg4_on=1;
}

void zeroDecimalPlaceNumner(int num){
	led8seg4_on=0;
	show4digits(num);
	setPoint(0);
	led8seg4_on=1;
}

void oneDecimalPlaceNumber(int num){
	led8seg4_on=0;
	show4digits(num);
	setPoint(1);
	led8seg4_on=1;
}

void twoDecimalPlaceNumber(int num){
	led8seg4_on=0;
	show4digits(num);
	setPoint(2);
	led8seg4_on=1;
}

void threeDecimalPlaceNumber(int num){
	led8seg4_on=0;
	show4digits(num);
	setPoint(3);
	led8seg4_on=1;
}

//#define sleep_ms(t) busy_wait_us(t*1000)
void blink(int num){
	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	while (true) {
		for(int i=0;i<num;i++){
			gpio_put(LED_PIN, 1);
			sleep_ms(250);
			gpio_put(LED_PIN, 0);
			sleep_ms(250);
		}
		sleep_ms(750);
	}
}
