/*
 * max7221.c
 *
 * Created: 07/11/2017 15:28:45
 *  Author: Richard
 */


#include <stdlib.h>
#include <avr/io.h>

#include "board.h"
#include "spi.h"

#define SSEGMENT_IMPORT
#include "7segment.h"

#include "max7221.h"

/* Declarations of opaque structures */

struct ss_instance {
	const spi_interface *interface;
	int			        digits;
	char*				values;
	char				decimal_points;
};

/* Implementation of public functions */

ss_instance *ss_init( const spi_interface *interface, int digits ) {
	
	ss_instance *instance = malloc(sizeof(ss_instance));
	instance->interface = interface;
	instance->digits    = digits;
	instance->values	= malloc(digits);
	instance->decimal_points = 0;

	spi_setup(interface);
	spi_write(interface, SS_SCAN_LIMIT, 0x07); /* set to 8 digit display */
	spi_write(interface, SS_DECODE_MODE, 0xFF); /* set all digits to BCD */
	spi_write(interface, SS_INTENSITY, 0x07); /* set intensity to half */
	spi_write(interface, SS_DISPLAY_TEST, 0x00); /* Disable test mode */
	spi_write(interface, SS_SHUTDOWN, 0x01);  /* disable shutdown mode */

	return instance;
}

void ss_write_digit(const ss_instance *instance, uint8_t digit, uint8_t value) {
	if ( instance->decimal_points & (1<<digit) ) /* is decimal point set */
		value += 0x80;
	spi_write(instance->interface ,SS_D0 + digit, value);
	instance->values[digit] = value;
}

void ss_set_dp(ss_instance *instance, uint8_t digit) {
	instance->decimal_points |= (1<<digit);
	ss_write_digit(instance,digit,instance->values[digit]);
}

void ss_blank_digit(const ss_instance *instance, uint8_t digit) {
	spi_write(instance->interface  ,SS_D0 + digit, 0x0F);
	instance->values[digit] = 10;
}

void ss_write_int(const ss_instance *instance, int value) {
	int digit = instance->digits - 1;
	while (value != 0) {
		ss_write_digit(instance, digit, value % 10);
		value /= 10;
		digit--;
	}

	while (digit >= 0) {
		ss_blank_digit(instance, digit);
		digit--;
	} 
}
