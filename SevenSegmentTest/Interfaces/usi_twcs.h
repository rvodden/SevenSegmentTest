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

#ifdef TWO_WIRE_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif /* TWO_WIRE_IMPORT */

EXTERN spi_interface *tw_create_spi_interface(
	volatile uint8_t* scl_direction_register, /** the register which controls the direction of the SCL pin */
	volatile uint8_t* scl_port,				  /** the port for SCL (used for writing) */
	volatile uint8_t* scl_pin,				  /** the pin for SCL (used for reading) */
	         uint8_t  scl_bit,				  /** the bit number of SCL on the pin, the direction register and the port */

	volatile uint8_t* sda_direction_register, /** the register which controls the direction of the SDA pin */
	volatile uint8_t* sda_port,				  /** the port for SDA (used for writing) */
	volatile uint8_t* sda_pin,				  /** the pin for SDA (used for reading) */
	         uint8_t  sda_bit,				  /** the bit number of SDA on the pin, the direction register and the port */

	volatile uint8_t* cs_direction_register, /** the register which controls the direction of the CS pin */
	volatile uint8_t* cs_port,				  /** the port for CS (used for writing) */
	volatile uint8_t* cs_pin,				  /** the pin for CS (used for reading) */
			 uint8_t  cs_bit,				  /** the bit number of CS on the pin, the direction register and the port */

	volatile uint8_t* usi_data_register,	  /** the universal serial interface data register */
	volatile uint8_t* usi_control_register,	  /** the universal serial interface control register */
	volatile uint8_t* usi_status_register	  /** the universal serial interface status register */
);

#undef TWO_WIRE_IMPORT
#undef EXTERN

#endif /* TWO_WIRE_H_ */