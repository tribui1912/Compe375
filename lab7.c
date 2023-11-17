/*
 * interrupt period: X+1 = 5+1 = 6ms 
 * Potentiometer A/D channel: Y%6 = 3%6 = 3 
 * PWM freq: (Z+1)*100 = 700 => f = 1.42ms
 * REDID 828135536
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#define LEDON PORTB |= (1<<5) //led on shortcut
#define LEDOFF PORTB &= ~(1<<5) //led off shortcut

void timer_init()
{
	TCCR0A |= (1 << WGM01); //setup CTC mode
	TCCR0B |= (1 << CS02); // prescaler = 256
	OCR0A = 89; // 256 = 4.096ms => 88.75 = 1.42ms => round to 89
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B); //enable compare a interrupt
	

	TCCR2A |= (1 << WGM21); //setup CTC mode
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); //prescaler 1024
	OCR2A = 51; // 255 count = 16.384ms => 6ms = 51
	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B); //enable compare match for A and B interrupt
}

void adc_init(void)
{
	DDRC &= ~(1 << PINC3); //set input pin as PORTC pin 1 Y%6
	ADMUX |= (1 << REFS0); //set vcc reference
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC auto trigger and conversion complete interrupt
}

ISR(TIMER0_COMPA_vect)
{
	LEDON;
}
ISR(TIMER0_COMPB_vect)
{
	LEDOFF;
}
ISR(TIMER2_COMPA_vect)
{
	ADCSRA |= (1 << ADSC); //timer 0 overflow occurs, start conversion
}
ISR(ADC_vect)
{
	uint16_t variable = ADC;
	OCR0B = variable * (OCR0A / 1024.0);
}



int main(void)
{
	timer_init();
	adc_init();
	sei(); //enable global interupt
	DDRB |= (1 << 5); //set out put led pin

    while (1) 
    {
	}
}




