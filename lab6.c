// XYZ = 536
// X = 5 => timer0 period 5+1 = 6ms
// Z = 6 => PWM freq (6+1)*100 = 700Hz => 1.428ms
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

// Define LED on and off shortcut
#define LEDON PORTB |= (1<<5) //LED on
#define LEDOFF PORTB &= ~(1<<5) //LED off

void keypad_init()
{
	DDRD |= ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)); //rows of keyhpad PD4 to PD7
	DDRB &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); //columns of keypad PB0 to PB3
	
	PORTB |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); //set PB0 to PB3 as pull-up registers
	PORTD |= ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
}

void timer0_init() //initialize Timer0 
{
	TCCR0A |= (1 << WGM01); //set Timer0 mode to CTC
	TCCR0B |= (1 << CS02) | (1 << CS00); //prescaler set to 1024
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 96; // X=5 => 6ms => 37.5% => 245*0.375 = 96
}

void timer2_init() //initialize Timer1
{
	TCCR2A |= (1 << WGM21);
	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B); //enable A and B interupt
	TCCR2B |= (1 << CS02); //prescaler set to 256
	OCR2A = 89; // 1.428 / 4.096 = 34.86% => 0.3486 * 256 = 89
}

int main(void) {
	keypad_init();
	timer0_init();
	timer2_init();
	sei(); // Enable global interrupts

	DDRB |= (1 << PORTB5); // LED pin as output

	while(1) {
		// Main loop, interrupts will take care of everything
	}
}

ISR(TIMER2_COMPA_vect) //ISR turn on LED
{
	LEDON;
}
ISR(TIMER2_COMPB_vect) //ISR turn off LED
{
	LEDOFF;
}

ISR(TIMER0_COMPA_vect) //ISR to scan keypad and adjusts the PWM duty cycle
{
	int keypad [4][4] = {{1,2,3,0},
						{4,5,6,0},
						{7,8,9,0},
						{0,0,0,0}};
	
	//Function to scan keypad
	for (int i = 4; i <= 7; i++)
	{
		PORTD &= ~(1 << i); //set one row to low at a time
		for (int j = 0; j <= 3; j++)
		{
			if (!(PINB & (1 << j)))
			{
				int dutycyle = keypad[i-4][j];
				OCR2B = OCR2A / 10 * dutycyle;
			}
			PORTB |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
		}
		PORTD |= (1 << i); //put row back to high to reset						
	}
}
