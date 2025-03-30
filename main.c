#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_ese.h"

volatile uint16_t adc_value = 0;


void init_adc() {
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADMUX &= ~(1 << MUX0);  
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t acd_read(uint8_t port) {
	
	ADMUX = (ADMUX & 0xF8) | (port & 0x07);	
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	
	return ADCH;
}

void init_timer1() {
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  
	OCR1A = 3125;  
	TIMSK1 |= (1 << OCIE1A);  
}


ISR(TIMER1_COMPA_vect) {
	ADCSRA |= (1 << ADSC);  
}


ISR(ADC_vect) {
	adc_value = ADC >> 1;  
	putUART((uint8_t)adc_value);  
}

int main(void) {
	init_uart(9600);  
	init_adc(); 	
	init_timer1();  
	sei();  

	while (1) {
		
	}
}
