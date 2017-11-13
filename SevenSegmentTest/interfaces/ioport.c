/*
 * ioport.c
 *
 * Created: 11/11/2017 08:42:24
 *  Author: Richard
 */ 

 #include <stdlib.h>
 #include <stdint.h>

 #define IOPORT_IMPORT
 #include "ioport.h"

 /* Private Macros and Constants */

 /* Declarations of opaque structures */

 struct ioport_interface {
	volatile uint8_t* direction_register; /** the register which controls the direction of the pin */
	volatile uint8_t* port;				  /** the port (used for writing) */
	volatile uint8_t* pin;				  /** the pin  (used for reading) */
			 uint8_t  bit;				  /** the bit number on the pin, the direction register and the port */
 };

 /* Private Functions */

 /* Implementation of public functions */

ioport_interface* ioport_construct (
	volatile uint8_t* direction_register,
	volatile uint8_t* port,
	volatile uint8_t* pin,
	uint8_t  bit
)
{
	ioport_interface* interface = malloc(sizeof(ioport_interface));
	interface->direction_register = direction_register;
	interface->port = port;
	interface->pin = pin;
	interface->bit = bit;

	return interface;
}

inline void ioport_setlow(ioport_interface* interface) {
	*(interface->port) &= ~(1 << interface->bit);
}

inline void ioport_sethigh(ioport_interface* interface) {
	*(interface->port) |= (1 << interface->bit);
}

inline void ioport_toggle(ioport_interface* interface) {
	*(interface->port) ^= ( 1 << interface->bit);
}

inline void ioport_pause_until_high(ioport_interface* interface) {
	while (!( *(interface->pin) & (1 << interface->bit)));
}

inline void ioport_configure_as_input(ioport_interface* interface) {
	*(interface->direction_register) &= ~(1 << interface->bit);
}

inline void ioport_configure_as_output(ioport_interface* interface) {
	*(interface->direction_register) |= (1 << interface->bit);
}
 
