#include <avr/io.h>
#define F_CPU 16000000UL //16Mhz CPU
#define LEDON PORTB |= (1<<5) //LED on 
#define LEDOFF PORTB &= ~(1<<5) //LED off

int main(void)
{
	TCCR0A |= (1 << WGM01); //set timer mode to ctc
	TCCR0B |= (1 << CS02) | (1 << CS00); //set prescaler to 1024 
	OCR0A = 0xB9; //top value 10ms period 156-1 = 155 cycles
	OCR0B = 0x00; //duty cycle start with 0
	
	DDRB &= ~(1 << DDB7); //set SW0 as input - it used PB7
	DDRB |= (1 << DDB5); //set led0 as output - it used PB5
	PORTB |= (1 << DDB7); //set pull-up for button

	
	while(1)
	{
		if (!(PINB & (1 << 7)) && (OCR0B < OCR0A)) // check if button is pressed and if OCR0B < OCR0A
		{
			OCR0B++; //increase duty cycle
		}
		LEDON;
		while ((TIFR0 & (1 << OCF0B)) == 0){} //wait for OCR0B overflow
		TIFR0 |= ( 1 << OCF0B); //reset OCR0B overflow flag
		if (PINB & (1 << 7) && (OCR0B > 0)) //check if button is not pressed and OCR0B > 0
		{
			OCR0B--; //decrease duty cycle
		}
		LEDOFF;
		while ((TIFR0 & (1 << OCF0A)) == 0){} //wait for OCR0 overflow
		TIFR0 |= (1 << OCF0A); //reset OCR0A oveflow flag
	}
	
	
}
