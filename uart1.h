
#ifndef USART1_H
#define USART1_H


void UART1_Init  (unsigned int ubrr_value);
void UART1_Transmit (char data);
void UART1_strTransmit  (char * sdata);




#endif
