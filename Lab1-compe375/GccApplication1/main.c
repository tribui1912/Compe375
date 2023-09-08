#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void dot(void)
{
	PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
	_delay_ms(200); //delay 200ms for on time
	PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(200); //delay 200ms for between same part of letters
}
void dash(void)
{
	PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
	_delay_ms(600); //delay 600ms
	PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(200); //delay 200ms between same part of letters
}
void spclttr(void)
{
	_delay_ms(400); //delay 400ms (alread do 200ms as default for space between same part so just adding 400ms more instead
}
void spcwrd(void)
{
	_delay_ms(1200); //delay 1200ms (alread do 200ms as default for space between same part so just adding 1200ms more instead

}

int main(void)
{
	
	char str[] = "TRI BUI 828135536";
	
	/* Replace with your application code */
	DDRB |= (1<<DDB5); //0x20 (hex) // Set port bit B5 in data direction register to 1: an OUTput
	while(1){
		for(int i = 0; str[i] != '\0'; i++)
		{
			switch(str[i])
			{
				case 'A':
				case 'a':
				dot();dash();spclttr();
				break;
				
				case 'B':
				case 'b':
				dash();dot();dot();dot();spclttr();
				break;
				
				case 'C':
				case 'c':
				dash();dot();dash();dot();spclttr();
				break;
				
				case 'D':
				case 'd':
				dash();dot();dot();spclttr();
				
				case 'E':
				case 'e':
				dot();spclttr();
				break;
				
				case 'F':
				case 'f':
				dot();dot();dash();dot();
				break;
				
				case 'G':
				case 'g':
				dash();dash();dot();
				break;
				
				case 'H':
				case 'h':
				dot();dot();dot();dot();spclttr();
				break;
				
				case 'I':
				case 'i':
				dot();dot();spclttr();
				break;
				
				case 'J':
				case 'j':
				dot();dash();dash();dash();spclttr();
				break;
				
				case 'K':
				case 'k':
				dash();dot();dash();spclttr();
				break;
				
				case 'L':
				case 'l':
				dot();dash();dot();dot();spclttr();
				break;
				
				case 'M':
				case 'm':
				dash();dash();spclttr();
				break;
				
				case 'N':
				case 'n':
				dash();dot();spclttr();
				break;
				
				case 'O':
				case 'o':
				dash();dash();dash();spclttr();
				break;
				
				case 'P':
				case 'p':
				dot();dash();dash();dot();spclttr();
				break;
				
				case 'Q':
				case 'q':
				dash();dash();dot();dash();spclttr();
				break;
				
				case 'R':
				case 'r':
				dot();dash();dot();spclttr();
				break;
				
				case 'S':
				case 's':
				dot();dot();dot();spclttr();
				break;
				
				case 'T':
				case 't':
				dash();spclttr();
				break;
				
				case 'U':
				case 'u':
				dot();dot();dash();spclttr();
				break;
				
				case 'V':
				case 'v':
				dot();dot();dot();dash();spclttr();
				break;
				
				case 'W':
				case 'w':
				dot();dash();dash();spclttr();
				break;
				
				case 'X':
				case 'x':
				dash();dot();dot();dash();spclttr();
				break;
				
				case 'Y' :
				case 'y':
				dash();dot();dash();dash();spclttr();
				break;
				
				case 'Z':
				case 'z':
				dash();dash();dot();dot();spclttr();
				break;
				
				case '1':
				dot();dash();dash();dash();dash();spclttr();
				break;
				
				case '2':
				dot();dot();dash();dash();dash();spclttr();
				break;
				
				case '3':
				dot();dot();dot();dash();dash();spclttr();
				break;
				
				case '4':
				dot();dot();dot();dot();dash();spclttr();
				break;
				
				case '5':
				dot();dot();dot();dot();dot();spclttr();
				break;
				
				case '6':
				dash();dot();dot();dot();dot();spclttr();
				break;
				
				case '7':
				dash();dash();dot();dot();dot();spclttr();
				break;
				
				case '8':
				dash();dash();dash();dot();dot();spclttr();
				break;
				
				case '9':
				dash();dash();dash();dash();dot();spclttr();
				break;
				
				case '0':
				dash();dash();dash();dash();dash();spclttr();
				break;
				
				case ' ':
				spcwrd();
				break;

			}
			
		}
		
	}
}
