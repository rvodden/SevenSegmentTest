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
#include "adc.h"

#define BOARD_IMPORT
#include "board.h"

#include <util/delay.h>

void board_init() {
	int value;

	ioport_interface* led = ioport_construct(&DDRB,&PORTB,&PINB,PB1);
	ioport_configure_as_output(led);
	ioport_setlow(led);

	ioport_interface* scl = ioport_construct(&DDRB,&PORTB,&PINB,PB2);
	ioport_interface* sda = ioport_construct(&DDRB,&PORTB,&PINB,PB0);
	ioport_interface*  cs = ioport_construct(&DDRB,&PORTB,&PINB,PB4);

	spi_interface *interface = tw_create_spi_interface(scl,sda,cs,&USIDR,&USICR,&USISR);
	
	ss_instance *instance = ss_init(interface, 4);
	adc_setup(NULL);

	

	ss_set_dp(instance,2);
	
	while (1) {
		value = adc_read(NULL);
		ss_write_int(instance, value);
		ioport_toggle(led);
		_delay_ms(100);
	}
}