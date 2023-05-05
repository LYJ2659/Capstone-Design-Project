
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

void USART_Init(unsigned int baud)
{
	unsigned int ubrr;
	ubrr =  (16000000UL/(16*baud) ) -1;
	UBRR1H = (unsigned char) (ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	UCSR1B = (1<<RXEN0) | (1<<TXEN0);
	UCSR1C = (3<<UCSZ0);
}



void USART_Transmit(char data)
{
	while(!(UCSR1A & (1<<UDRE0)));
	UDR1 = data;
}

char USART_Receive()
{
	while(!(UCSR1A &(1<<RXC0)));
	return UDR1;
}

void USART_Transmit_String(char *str)
{
	while(*str != '\0')
		USART_Transmit(*str++);
}


void USART_Receive_String(char *str)
{
	while(1)
	{
		*str=USART_Receive();
		if(*str == 8)
			continue;
		else if((*str != 10) && (*str != 13))
			USART_Transmit(*str);
		else
		{
			*str='\0';
			break;
		}
		str++;
	}
}
