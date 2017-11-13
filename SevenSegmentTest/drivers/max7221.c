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

/* Private Macros and Constants */

#define SS_NOOP 		0x00
#define SS_D0   		0x01
#define SS_D1   		0x02
#define SS_D2   		0x03
#define SS_D3   		0x04
#define SS_D4   		0x05
#define SS_D5   		0x06
#define SS_D6   		0x07
#define SS_D7			0x08
#define SS_DECODE_MODE	0x09
#define SS_INTENSITY	0x0A
#define SS_SCAN_LIMIT	0x0B
#define SS_SHUTDOWN		0x0C
#define SS_DISPLAY_TEST	0x0F

/* Declarations of opaque structures */

struct ss_instance {
	const spi_interface *interface;
	int			        digits;
	char*				values;
	char				decimal_points;
};

/* Implementation of public functions */

ss_instance* max7221_ss_construct(const spi_interface *interface, int digits) {
	ss_instance *instance = malloc(sizeof(ss_instance));
	instance->interface = interface;
	instance->digits    = digits;
	instance->values	= malloc(digits);
	instance->decimal_points = 0;

	return instance;
}

void ss_setup( const ss_instance* instance ) {
	spi_setup(instance->interface);
	spi_write(instance->interface, SS_SCAN_LIMIT, 0x07); /* set to 8 digit display */
	spi_write(instance->interface, SS_DECODE_MODE, 0xFF); /* set all digits to BCD */
	spi_write(instance->interface, SS_INTENSITY, 0x07); /* set intensity to half */
	spi_write(instance->interface, SS_DISPLAY_TEST, 0x00); /* Disable test mode */
	spi_write(instance->interface, SS_SHUTDOWN, 0x01);  /* disable shutdown mode */
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
