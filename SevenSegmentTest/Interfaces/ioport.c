/*
 * ioport.c
 *
 * Created: 11/11/2017 08:42:24
 *  Author: Richard
 */ 

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
	volatile uint8_t* direction_register;
	volatile uint8_t* port;
	volatile uint8_t* pin;
	uint8_t  bit;
)
 