/*
 * max7221.c
 *
 * Created: 07/11/2017 15:28:45
 *  Author: Richard
 */


#include <stdlib.h>
#include <avr/io.h>

#include "board.h"
#include "SPI.h"

#define SSEGMENT_IMPORT
#include "7segment.h"

#include "max7221.h"

/* Declarations of opaque structures */

struct ss_instance {
	const SPI_interface *interface;
	int			        digits;
};

/* Implementation of public functions */

ss_instance *ss_init( const SPI_interface *interface, int digits ) {
	
	ss_instance *instance = malloc(sizeof(ss_instance));
	instance->interface = interface;
	instance->digits    = digits;

	SPI_Setup(interface);
	SPI_Write(interface, SS_SCAN_LIMIT, 0x07); /* set to 8 digit display */
	SPI_Write(interface, SS_DECODE_MODE, 0xFF); /* set all digits to BCD */
	SPI_Write(interface, SS_INTENSITY, 0x07); /* set intensity to half */
	SPI_Write(interface, SS_DISPLAY_TEST, 0x00); /* Disable test mode */
	SPI_Write(interface, SS_SHUTDOWN, 0x01);  /* disable shutdown mode */

	return instance;
}

void ss_write_digit(const ss_instance *instance, uint8_t digit, uint8_t value) {
	SPI_Write(instance->interface ,SS_D0 + digit, value);
}

void ss_blank_digit(const ss_instance *instance, uint8_t digit) {
	SPI_Write(instance->interface  ,SS_D0 + digit, 0x0F);
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
