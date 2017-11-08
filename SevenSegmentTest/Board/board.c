/*
 * board.c
 *
 * Created: 07/11/2017 17:24:40
 *  Author: Richard
 */ 

#include <stdlib.h>
#include <avr/io.h>

#include "SPI.h"
#include "7segment.h"
#include "usi_twcs.h"

#define BOARD_IMPORT
#include "board.h"

#include <util/delay.h>

void board_init() {
	SPI_interface *interface = tw_create_spi_interface(&DDRB,&PORTB,&PINB,PB2,&DDRB,&PORTB,&PINB,PB0,&DDRB,&PORTB,&PINB,PB3,&USIDR,&USICR,&USISR);
	ss_instance *instance = ss_init(interface, 4);

	int i = 0;
	while (1) {
		ss_write_int(instance, i++);
		_delay_ms(100);
	}
}