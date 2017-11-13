/*
 * board.c
 *
 * Created: 07/11/2017 17:24:40
 *  Author: Richard
 */ 

#include <stdlib.h>
#include <avr/io.h>

#include "SPI.h"
#include "max7221.h"
#include "7segment.h"
#include "usi_twcs.h"
#include "lm35.h"
#include "adc.h"
#include "temperature.h"

#define BOARD_IMPORT
#include "board.h"

#include <util/delay.h>

void board_init() {
	int value;

	/* wire up LED */
	ioport_interface* led = ioport_construct(&DDRB,&PORTB,&PINB,PB1);
	ioport_configure_as_output(led);

	/* wire up temperature sensor */
	adc_interface* adc = adc_construct(&ADMUX, &ADCSRA, &ADCSRB, &ADCH, &ADCL);
	adc_setup(adc);
	ts_instance* ts = lm35_ts_construct(adc);

	/* wire up 7-segment display */
	ioport_interface* scl = ioport_construct(&DDRB,&PORTB,&PINB,PB2);
	ioport_interface* sda = ioport_construct(&DDRB,&PORTB,&PINB,PB0);
	ioport_interface*  cs = ioport_construct(&DDRB,&PORTB,&PINB,PB4);
	spi_interface *interface = tw_create_spi_interface(scl,sda,cs,&USIDR,&USICR,&USISR);
	ss_instance *instance = max7221_ss_construct(interface, 4);
	ss_setup(instance);

	/* turn led off */
	ioport_setlow(led);
	ss_set_dp(instance,2);
	
	while (1) {
		value = ts_get_temperature(ts);
		ss_write_int(instance, value);
		ioport_toggle(led);
		_delay_ms(100);
	}
}