#define F_CPU 16000000UL //CPU clock 16MHz
#include <avr/io.h>
#include <util/delay.h>

// Define the row and column ports
#define ROWS_PORT     PORTD //Set PORTD as out put for row
#define COLS_PORT     PINB //Set PINB as input for column

#define ROWS_MASK     0xF0
#define COLS_MASK     0x0F

// Define the USART settings
#define BAUD 9600
#define BAUDRATE F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable receiver and transmitter
	UCSR0C = (1 << UCSZ01 ) | (1 << UCSZ00); // Set frame format: 8data, 2stop bit
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1<<UDRE0)));    // Wait for empty transmit buffer
	UDR0 = data;                      // Put data into buffer, sends the data
}

char scanKeypad() {
	char keypad[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};

	for (uint8_t r = 0; r < 4; r++) {
		ROWS_PORT = (ROWS_PORT & ~ROWS_MASK) | (~(1 << (r + 4)) & ROWS_MASK);  // Set one row low at a time

		for (uint8_t c = 0; c < 4; c++) {
			if (!(COLS_PORT & (1 << c))) {
				return keypad[r][c]; // Return the key pressed
			}
		}
	}

	return 0; // No key pressed
}

int main(void) {
	DDRD |= ROWS_MASK;    // Set D4-D7 as outputs for rows
	DDRB &= ~COLS_MASK;   // Set B0-B3 as inputs for columns
	COLS_PORT |= COLS_MASK;   // Enable pull-ups for B0-B3 columns

	USART_Init(BAUDRATE);

	while (1) {
		char key = scanKeypad();
		if (key) {
			USART_Transmit(key);
			_delay_ms(50);  // Debounce delay
		}
	}
}
