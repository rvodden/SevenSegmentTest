/*
 * SevenSegmentTest.c
 *
 * Created: 07/11/2017 10:09:35
 * Author : Richard
 */ 

#include "board.h"
#include "7segment.h"
#include "SPI.h"

#include <avr/io.h>
#include <util/delay.h>

/* register constants */

int main(void)
{
	DDRB |=	(1<<LED);
	

	board_init();

	/* Replace with your application code */
    while (1);
}

