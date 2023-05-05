#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart1.h"

void UART1_Init  (unsigned int ubrr_value)
{
	UCSR1A = 0x00;
	UCSR1B = 0b10011000;
	UCSR1C = 0b00000110;
	UBRR1H = (uint8_t)(ubrr_value >> 8);
    UBRR1L = (uint8_t)ubrr_value;
}

void UART1_Transmit (char data)
{
	while(!(UCSR1A & (1 << UDRE1 )) );
	UDR1 = data; // 데이터 전송
}

void UART1_strTransmit  (char *sdata)
{
	unsigned char i = 0;
	
	while(sdata[i] != '\0')
	{
		UART1_Transmit (sdata[i]);
		i++;
	}
}
