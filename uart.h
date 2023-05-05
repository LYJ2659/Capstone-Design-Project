
#ifndef USART1_H
#define USART1_H

void USART_Init(unsigned int baud);


void USART_Transmit(char data);

char USART_Receive();


void USART_Transmit_String(char *str);


void USART_Receive_String(char *str);

#endif
