/**
 * \file TwoWire.c
 * \brief A two wire implementation of the SPI service.                                                                    
 *
 */

#include "board.h"

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define SPI_IMPORT
#include "SPI.h"

#define TWO_WIRE_IMPORT
#include "usi_twcs.h"

/* Private Macros and Constants */

#define T2_TWI ((F_CPU * 4700) / 1000000) + 1 // >4,7us
#define T4_TWI ((F_CPU * 4000) / 1000000) + 1 // >4,0us

/* Declarations of opaque structures */

struct SPI_interface
{
	volatile uint8_t* scl_direction_register; /** the register which controls the direction of the SCL pin */
	volatile uint8_t* scl_port;				  /** the port for SCL (used for writing) */
	volatile uint8_t* scl_pin;				  /** the pin for SCL (used for reading) */
	         uint8_t  scl_bit;				  /** the bit number of SCL on the pin; the direction register and the port */

	volatile uint8_t* sda_direction_register; /** the register which controls the direction of the SDA pin */
	volatile uint8_t* sda_port;				  /** the port for SDA (used for writing) */
	volatile uint8_t* sda_pin;				  /** the pin for SDA (used for reading) */
	         uint8_t  sda_bit;				  /** the bit number of SDA on the pin; the direction register and the port */

	volatile uint8_t* cs_direction_register;  /** the register which controls the direction of the CS pin */
	volatile uint8_t* cs_port;				  /** the port for CS (used for writing) */
	volatile uint8_t* cs_pin;				  /** the pin for CS (used for reading) */
			 uint8_t  cs_bit;				  /** the bit number of CS on the pin; the direction register and the port */

	volatile uint8_t* usi_data_register;	  /** the universal serial interface data register */
	volatile uint8_t* usi_control_register;	  /** the universal serial interface control register */
	volatile uint8_t* usi_status_register;	  /** the universal serial interface status register */
};

/* Private Functions */

 void SPI_Push(const SPI_interface *interface, uint8_t data) {
	 *(interface->scl_port) &= ~(1 << interface->scl_bit);
	 *(interface->usi_data_register) = data;

	 *(interface->usi_status_register) = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC)| /* Clear flags */
										 (0x0 << USICNT0);											  /* set USI to shift 8 bits */

	 do {
		 _delay_us(T2_TWI);
		 *(interface->usi_control_register) |= (1 << USITC); /* toggle clock - positive edge */
		 while (!(*(interface->scl_pin) & (1 << interface->scl_bit)));  /* wait for SCL to go high. */
		 _delay_us(T4_TWI);
		 *(interface->usi_control_register) |= (1 << USITC); /* toggle clock - positive edge */
	 } while (!(*(interface->usi_status_register) & (1 << USIOIF)));

	 /* do reading here when the time comes */

	 _delay_us(T2_TWI);
 }

/* Implementation of public functions */

SPI_interface *tw_create_spi_interface( 
	volatile uint8_t* scl_direction_register, 
	volatile uint8_t* scl_port, 
	volatile uint8_t* scl_pin, 
	         uint8_t  scl_bit, 

	volatile uint8_t* sda_direction_register, 
	volatile uint8_t* sda_port, 
	volatile uint8_t* sda_pin, 
	         uint8_t  sda_bit,

    volatile uint8_t* cs_direction_register, 
    volatile uint8_t* cs_port,				  
    volatile uint8_t* cs_pin,				  
			 uint8_t  cs_bit,				
			   
	volatile uint8_t* usi_data_register, 
	volatile uint8_t* usi_control_register, 
	volatile uint8_t* usi_status_register 
	)
{
	SPI_interface *interface = malloc(sizeof(SPI_interface));

	interface->scl_direction_register = scl_direction_register;
	interface->scl_port = scl_port;
	interface->scl_pin = scl_pin;
	interface->scl_bit = scl_bit;

	interface->sda_direction_register = sda_direction_register;
	interface->sda_port = sda_port;
	interface->sda_pin = sda_pin;
	interface->sda_bit = sda_bit;

	interface->cs_direction_register = cs_direction_register;
	interface->cs_port = cs_port;
	interface->cs_pin = cs_pin;
	interface->cs_bit = cs_bit;

	interface->usi_data_register = usi_data_register;
	interface->usi_control_register = usi_control_register;
	interface->usi_status_register = usi_status_register;
	return interface;
}

 void SPI_Setup(const SPI_interface *interface) {
	 /* de-select the correct chip */
	 *(interface->cs_direction_register) |= (1 << interface->cs_bit);

	 *(interface->scl_direction_register) |= (1 <<interface->scl_bit);
	 *(interface->sda_direction_register) |= (1 <<interface->sda_bit);

	 /* Pre-load register with 1 (as pull-ups mean a floating line is high). */
	 *(interface->usi_data_register) = 0xFF;

	 *(interface->usi_control_register) = (0 << USISIE) | (0 << USIOIE) |				  /* Disable interrupts.         */
										  (1 << USIWM1) | (0 << USIWM0) |				  /* Set USI in Two-wire mode.   */
										  (1 << USICS1) | (0 << USICS0) | (1 << USICLK) | /* Software clock              */
										  (0 << USITC);

	 *(interface->usi_status_register) = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | /* clear flags   */
										 (0x0 << USICNT0);											   /* reset counter */
 }

 void SPI_Write(const SPI_interface *interface, uint8_t addr, uint8_t data) {

	 /* at this stage we should detect if this is a read or write operation - but I'm only doing writes right now */

	 /* select appropriate chip */
	 *(interface->cs_port) &= ~(1<<interface->cs_bit); 

	 /* release SCL so that a start condition can be raised */
	 *(interface->scl_port) |= (1 << interface->scl_bit);
	 /* chug around this until the pin rises */
	 while (!( *(interface->scl_pin) & (1 << interface->scl_bit)))
	 
	 /* start condition */
	 *(interface->sda_port) &= ~(1 << interface->sda_bit); // pull SDA low.
	 _delay_us(T2_TWI);
	 *(interface->scl_port) &= ~(1 << interface->scl_bit); // pull SCL low.
	 *(interface->sda_port) |=  (1 << interface->sda_bit); // release SDA.

	 /* should probably do something to support messages longer than one byte */

	 /* write address */
	 SPI_Push(interface, addr);
	 SPI_Push(interface, data);

	 /* raise appropriate chip select pin */
	 PORTB |= (1 << interface->cs_bit); /* de-select SS display driver */
 }