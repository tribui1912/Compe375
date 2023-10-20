#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void keypad_init(void)
{
	DDRD |= ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)); //rows of keyhpad PD4 to PD7
	DDRB &= ~((1 << 0) & ~(1 << 1) & ~(1 << 2) & ~(1 << 3)); //columns of keypad PB0 to PB3
	
	PORTB |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); //set PB0 to PB3 as pull-up registers
	PORTD |= ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
	DDRD |= (1 << 1); //Headphone pin: D1 = red, GND = yellow
	DDRB |= (1 << PORTB5);
}

void timer_init(int freq) //initialize Timer0 to create square signal wave based on freq
{
	TCCR0A |= (1 << WGM01); //set Timer0 mode to CTC
	OCR0A = freq; //set ending freq
	OCR0B = freq * 0.5; //set starting freq
	TCCR0B |= (1 << CS02); //prescaler set to 256
}

void keypad_note(int freq, int a)
{
	timer_init(freq); //timer initialize
	while(!(PINB & (1 << a))) //check if keyupad button corresponding to the column a is pressed
	{
		while((TIFR0 & (1 << OCF0B)) == 0) //wait for OCR0B overflow
		{
			PORTD |= (1 << 1); //wait for the timer to match OCR0B then set PD1 high
		}
		TIFR0 |= (1 << OCF0B); //reset OCR0B overflow
		
		while((TIFR0 & (1 << OCF0A)) == 0) //wait for OCR0A overflow
		{
			PORTD &= ~(1 << OCF0A);	// wait for the timer to match OCR0A then set PD1 to low
		}
		TIFR0 |= (1 << OCF0A); //reset OCR0A overflow
	}
}

int main(void)
{
    keypad_init();
	const int keypad [4][4] = {{141,133,126,119},
							   {112,105,99,94},
							   {88,83,79,74},
							   {70,66,59,55}};
				//f = 16,000,000 / [256 * (1 + OCR0A)] => derived from T = precaler * (1+OCR0A) / F_CPU.
				//target f = 440 * (2 ^ 1/12) ^ x (x =0,1,2,...)

    while (1) 
    {
		for (int i = 4; i <= 7; i++)
		{
			PORTD &= ~(1 << i); //set one row to low at a time
			for (int j = 0; j <= 3; j++)
			{
				if (!(PINB & (1 << j)))
				{
					keypad_note(keypad[i-4][j],j);
				}
				PORTB |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
			}
			PORTD |= (1 << i); //put row back to high to reset
		}
    }
}
