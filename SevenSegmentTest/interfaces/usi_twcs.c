/**
 * \file TwoWire.c
 * \brief A two wire implementation of the SPI service.                                                                    
 *
 */

#include "board.h"
#include "ioport.h"

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define SPI_IMPORT
#include "spi.h"

#define TWO_WIRE_IMPORT
#include "usi_twcs.h"

/* Private Macros and Constants */

#define T2_TWI ((F_CPU * 4700) / 1000000) + 1 // >4,7us
#define T4_TWI ((F_CPU * 4000) / 1000000) + 1 // >4,0us

/* Declarations of opaque structures */

struct spi_interface
{
	ioport_interface* scl;
	ioport_interface* sda;
	ioport_interface* cs;

	volatile uint8_t* usi_data_register;	  /** the universal serial interface data register */
	volatile uint8_t* usi_control_register;	  /** the universal serial interface control register */
	volatile uint8_t* usi_status_register;	  /** the universal serial interface status register */
};

/* Private Functions */

/* prototypes */
void SPI_Push(const spi_interface *interface, uint8_t data);

/* declarations */
void SPI_Push(const spi_interface *interface, uint8_t data) {
	ioport_setlow(interface->scl);
	*(interface->usi_data_register) = data;
	*(interface->usi_status_register) = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC)| /* Clear flags */
										 (0x0 << USICNT0);											  /* set USI to shift 8 bits */

	do {
		_delay_us(T2_TWI);
		*(interface->usi_control_register) |= (1 << USITC); /* toggle clock - positive edge */
		ioport_pause_until_high(interface->scl);
		_delay_us(T4_TWI);
		*(interface->usi_control_register) |= (1 << USITC); /* toggle clock - negative edge */
	} while (!(*(interface->usi_status_register) & (1 << USIOIF)));

	/* do reading here when the time comes */

    _delay_us(T2_TWI);
}

/* Implementation of public functions */

spi_interface *tw_create_spi_interface( 
	ioport_interface* scl,
	ioport_interface* sda,
	ioport_interface* cs,				
			   
	volatile uint8_t* usi_data_register, 
	volatile uint8_t* usi_control_register, 
	volatile uint8_t* usi_status_register 
	)
{
	spi_interface *interface = malloc(sizeof(spi_interface));

	interface->scl = scl;
	interface->sda = sda;
	interface->cs =  cs;

	interface->usi_data_register = usi_data_register;
	interface->usi_control_register = usi_control_register;
	interface->usi_status_register = usi_status_register;
	return interface;
}

 void spi_setup(const spi_interface *interface) {
	  ioport_configure_as_output(interface->scl);
	  ioport_configure_as_output(interface->sda);
	  ioport_configure_as_output(interface->cs);

	  /* de-select chip */
	  ioport_sethigh(interface->cs);

	 /* Pre-load register with 1 (as pull-ups mean a floating line is high). */
	 *(interface->usi_data_register) = 0xFF;

	 *(interface->usi_control_register) = (0 << USISIE) | (0 << USIOIE) |				  /* Disable interrupts.         */
										  (1 << USIWM1) | (0 << USIWM0) |				  /* Set USI in Two-wire mode.   */
										  (1 << USICS1) | (0 << USICS0) | (1 << USICLK) | /* Software clock              */
										  (0 << USITC);

	 *(interface->usi_status_register) = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | /* clear flags   */
										 (0x0 << USICNT0);											   /* reset counter */
 }

 void spi_write(const spi_interface *interface, uint8_t addr, uint8_t data) {

	 /* at this stage we should detect if this is a read or write operation - but I'm only doing writes right now */

	 /* select appropriate chip */
	 ioport_setlow(interface->cs);

	 /* release SCL so that a start condition can be raised */
	 ioport_sethigh(interface->scl);

	 /* wait until SCL comes high in case a slave is holding it low */
	 ioport_pause_until_high(interface->scl);
	 
	 /* start condition */
	 ioport_setlow(interface->sda);
	 _delay_us(T2_TWI);
	 ioport_setlow(interface->scl);
	 ioport_sethigh(interface->sda);

	 /* should probably do something to support messages longer than one byte */

	 /* write address */
	 SPI_Push(interface, addr);
	 SPI_Push(interface, data);

	 /* raise appropriate chip select pin */
	 ioport_sethigh(interface->cs);
 }
