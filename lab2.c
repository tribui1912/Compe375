#define F_CPU 16000000UL //CPU clock 16MHz
#define BAUD 9600 //define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) //set baud rate value for UBRR

#include <avr/io.h>
#include <util/delay.h>


void usart_ini(void)
{
	//clear usart baud rate 
	UBRR0H &= 0;
	//set baud rate
	UBRR0H = (BAUDRATE >> 8); //shift the register right by 8 bit, because the high 8 bit is reserved
	UBRR0L = BAUDRATE;
	//enable transmitter
	UCSR0B = (1 << TXEN0);
	//set frame format: 8 data bits, 1 stop bits
	UCSR0C = (0 << UCSR0B) | (3 << UCSZ00);
}

void usart_transmit(unsigned char data)
{
	//wait for the transmit buffer to be empty
	while(!(UCSR0A & (1 << UDRE0)));
	//send data into buffer to send the data
	UDR0 = data;
}


void sendstr(const char* str)
{
	while(*str)
	{
		usart_transmit(*str++);
		_delay_ms(500);
	}
}

int main(void)
{
	usart_ini();
	while (1)
	{
		sendstr("828135536\r\n"); // \r\n for end of line

	}
	return 0;
}
