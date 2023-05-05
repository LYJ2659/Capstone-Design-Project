/*
lcdpcf8574 lib sample

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcdpcf8574.h"
#include "uart1.h"
#include "rstack.h"

#define SERIAL_SIZE  64
#define SERIAL_SOF   0
#define SERIAL_SENSOR_COMMT_START 3

#define PIN(x) (*(&x - 2))    /* address of input register of port x */

volatile unsigned char sindex = 0;
volatile unsigned char sflag = 1;
volatile char serialBuf[SERIAL_SIZE]; 
volatile unsigned char ch;

volatile unsigned char count;
volatile unsigned char start = 0;
volatile unsigned char btn = 0;

ISR (INT4_vect) 
{
	btn = 1;
}

ISR (USART1_RX_vect)
{
	if(sindex >= 18) sindex = 0;
	
	ch = UDR1;
	 
	if(ch == '<')
	{
		sindex = 0;	
		sflag = 1;
		serialBuf [sindex++] = ch;
	
	}
	
	else if(ch == '>')	
	{
		serialBuf [sindex] = '\0';
		sflag = 0;
	}
	
	else
	{
		serialBuf [sindex++] = ch;
		sflag = 1;
	}
} 


void InitADC(void)
{
    ADMUX |= (1<<REFS0);    
    ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); // ENABLE ADC, PRESCALER 128
}

uint16_t readadc(uint8_t ch)
{
    ch &= 0b00000111;         // ANDing to limit input to 7
    ADMUX = (ADMUX & 0xf8) | ch;  // Clear last 3 bits of ADMUX, OR with ch
    ADCSRA |= (1<<ADSC);        // START CONVERSION
    while((ADCSRA) & (1<<ADSC));    // WAIT UNTIL CONVERSION IS COMPLETE
    return(ADC);        	// RETURN ADC VALUE
}

int main(void)
{
	char a[20], b[20], c[20];   
	int x, y;
	char dir;
	uint8_t led = 0;

	UART1_Init(103);
    	InitADC();

	createStack(100);
	 
	PORTE |= 0x10; // Connecting a PULL UP resistor
	EICRB |= 0x20; // Interrupt trigger method setting: Falling edge	
	EIMSK |= 0x10; // Interrupt enable setting

	sei();

    	lcd_init(LCD_DISP_ON_BLINK);
    	lcd_led(led); //set led: led = 0 -> led on, led = 1 -> led off

    	while(1) 
	{
		
		lcd_home();
	
		x = readadc(0);      	// READ ADC VALUE FROM PF.0
        	y = readadc(1);      	// READ ADC VALUE FROM PF.1
        	x = x - 512;         	// x = -4
        	y = y - 512;   		// y = 14
	
		x = x * (-1);
		
		if(x > -10 && x < 20) x = 0;
		if(y > -10 && y < 20) y = 0;

		itoa(x,a,10);    
        	itoa(y,b,10);
		
		lcd_puts("x=");     // DISPLAY THE RESULTS ON LCD
        	lcd_gotoxy(2,0);
        	lcd_puts(a);
		lcd_puts("     ");

		if(x > 0) sprintf(c,"<%c1%c>", '1', 'r');
		if(x < 0) sprintf(c,"<%c1%c>", '1', 'l');
			
		lcd_gotoxy(7,0);
        	lcd_puts("y=");
        	lcd_gotoxy(9,0);
        	lcd_puts(b);
		lcd_puts("     ");

		if(y > 0) sprintf(c,"<%c1%c>", '1', 's');
		if(y < 0) sprintf(c,"<%c1%c>", '1', 'b');

		if(y == 0 && x == 0) sprintf(c,"<%c1%c>", '1', 'q');

		if(btn == 1)
		{

			if(c[2] == '1')
			{
				if(c[3] == 'q')
				{
					dir = pop(stack);
					lcd_gotoxy(14,1);
					lcd_putc(dir);					
				}
				else
				{
					UART1_strTransmit(c);
				
					if(c[3] == 'b' || c[3] == 's')
					{
						
						
					}
				
					else  
					{	
						push(stack, c[3]);
						_delay_ms(500);
					}
				}
			
			}

			
			btn = 0;
		}
		
		if(sflag == 0) // serial rx : when remote controller received all strings from the RC car
		{
			 
			lcd_gotoxy(0,1);
			lcd_puts("                ");
			lcd_gotoxy(0,1);
			lcd_puts((const char *)&serialBuf[SERIAL_SENSOR_COMMT_START]);
			sflag = 1;
		}

	}
}


