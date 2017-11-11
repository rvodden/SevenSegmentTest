/*
 * two_wire.h
 *
 * Created: 07/11/2017 16:58:25
 *  Author: Richard
 *
 * implementation of the SPI module using the AVR USI and a chip select line
 *
 */ 


#ifndef TWO_WIRE_H_
#define TWO_WIRE_H_

#include "SPI.h"
#include "ioport.h"

#ifdef TWO_WIRE_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif /* TWO_WIRE_IMPORT */

EXTERN spi_interface *tw_create_spi_interface(
	
	ioport_interface* scl,
	ioport_interface* sda,
	ioport_interface* cs,

	volatile uint8_t* usi_data_register,	  /** the universal serial interface data register */
	volatile uint8_t* usi_control_register,	  /** the universal serial interface control register */
	volatile uint8_t* usi_status_register	  /** the universal serial interface status register */
);

#undef TWO_WIRE_IMPORT
#undef EXTERN

#endif /* TWO_WIRE_H_ */