#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL //16 MHz CPU
#define MAX_DUTY_CYCLE 155
#define LEDON PORTB |= (1<<5) //LED on
#define LEDOFF PORTB &= ~(1<<5) //LED off

void timer0_init()
{
	TCCR0A |= (1 << WGM01); //set timer mode to ctc
	TCCR0B |= (1 << CS02) | (1 << CS00); //set prescaler to 1024
	OCR0A = 0x4D; //top value 5ms period 78-1 = 77 cycles
	OCR0B = 0x00; //duty cycle start with 0
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B); //Enable compare match A and B interrupt
}

uint8_t EEMEM address = 0;
uint8_t duty_cycle;
uint8_t EEMEM first_run_flag = 1;
uint8_t first_run;

int main(void)
{
	DDRB |= (1 << DDB5); //Set led at pin B5 output (onboard)
	timer0_init();
	first_run = eeprom_read_byte(&first_run_flag);
    if (first_run)
    {
	    // First run, initialize duty cycle and set flag to false
	    duty_cycle = 0;
	    eeprom_update_byte(&address, duty_cycle);
	    eeprom_update_byte(&first_run_flag, 0);
    }
    else //If not run the loop to check for increase or reset
    {
		duty_cycle = eeprom_read_byte(&address);

		if (duty_cycle >= 100)
		{
			duty_cycle = 0;
		} else
		{
			duty_cycle = duty_cycle + 10;
		}
		eeprom_update_byte(&address, duty_cycle); //Write into eeprom for saving
	}
	OCR0B = OCR0A * (duty_cycle / 100.0); //Update duty cycle for led
	sei(); //Enable global interupt
	while(1)
	{
		
	}
}

ISR(TIMER0_COMPA_vect)
{
	LEDON;
}
ISR(TIMER0_COMPB_vect)
{
	LEDOFF;
}
