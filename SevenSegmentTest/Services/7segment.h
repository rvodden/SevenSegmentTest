/*
 * 7segment.h
 *
 * @copyright 2017 Richard Vodden
 * @license GPL 3.0
 * @author Richard Vodden <richard@vodden.com>
 * @file
 */

#ifndef SSEGMENT_H_
#define SSEGMENT_H_

#include <stdint.h>

#include "SPI.h"

#ifdef SSEGMENT_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Types */

typedef struct ss_instance ss_instance; 

/* Function Prototypes */

/**
 * Sets the display of a particular digit to be the decimal representation
 * of the value.
 
 * \param instance instance of the driver struct to refer to
 * \param digit the digit on which the value should be displayed
 * \param value the value which should be displayed on the digit                                                                     
 */
EXTERN void ss_write_digit( const ss_instance *instance, uint8_t digit, uint8_t value);

/**
 * Turns off all segments of a digit.
 *
 * \param instance instance of the driver struct to refer to
 * \param digit the digit which should have its segments turned off                                                                   
 */
EXTERN void ss_blank_digit( const ss_instance *instance, uint8_t digit);

/**
 * Writes an integer to the 7 segment displays. Those digits which
 * are not needed are blanked.
 *
 * \param instance instance of the driver struct to refer to
 * \param value the value which should be displayed.                                                                     
 */
EXTERN void ss_write_int(const ss_instance *instance, int value);

/**
 * Initializes the 7 segment display module.                                                                    
 */
EXTERN ss_instance* ss_init( const spi_interface *interface, int digits );

#undef SSEGMENT_IMPORT
#undef EXTERN

#endif /* SSEGMENT_H_ */