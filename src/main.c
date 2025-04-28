#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "led8seg4.h"

#define IO_ADC_PORT 26
#define IO_ADC_INPUT 0

#define IO_PWM_PORT 0
#define IO_PWM_SLICE 0
#define IO_PWM_CHANNEL PWM_CHAN_A

int main() {
	// Initialize display
	led8seg4_init();
	// Initialize ADC
	adc_init();
	gpio_init(IO_ADC_PORT);
	gpio_set_dir(IO_ADC_PORT, GPIO_IN);
	gpio_disable_pulls(IO_ADC_PORT);
	adc_gpio_init(IO_ADC_PORT);
	adc_select_input(IO_ADC_INPUT);
	// Initialize PWM
	gpio_set_function(IO_PWM_PORT, GPIO_FUNC_PWM);
	pwm_set_clkdiv(IO_PWM_SLICE,23.077); // 100 Hz PWM
	pwm_set_wrap(IO_PWM_SLICE, 65000);   // 65000 cycles PWM
	pwm_set_chan_level(IO_PWM_SLICE, IO_PWM_CHANNEL, 0);
	pwm_set_enabled(IO_PWM_SLICE, true);
	
	while(true){
		int adcsum=0;
		for(int i=0;i<64;i++){
			sleep_ms(5);
			adcsum+=adc_read()-38;
		}
		int adc=adcsum>>6;
		if (adc<0) adc=0;
		if (4000<adc) adc=4000;
		adc>>=2;
		oneDecimalPlaceNumber(adc);
		pwm_set_chan_level(IO_PWM_SLICE, IO_PWM_CHANNEL, 65*adc);
	}
}
