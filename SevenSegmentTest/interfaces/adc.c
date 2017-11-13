/*
 * adc.c
 *
 * Created: 10/11/2017 17:43:30
 *  Author: Richard
 */ 

#include <stdlib.h>
#include <avr/io.h>

#include "7segment.h"

#define ADC_IMPORT
#include "adc.h"

/* Private Macros and Constants */

/* Declarations of opaque structures */

struct adc_interface {
	volatile uint8_t *mux_selection_register;
	volatile uint8_t *control_a_register;
	volatile uint8_t *control_b_register;
	volatile uint8_t *high_data_register;
	volatile uint8_t *low_data_register;
};

/* Private Functions */

void inline adc_wait_for_conversion(const adc_interface* interface) {
	while (*(interface->control_a_register) & (1<<ADSC));
}

/* Implementation of public functions */

adc_interface* adc_construct( 
	volatile uint8_t *mux_selection_register, 
	volatile uint8_t *control_a_register, 
	volatile uint8_t *control_b_register, 
	volatile uint8_t *high_data_register, 
	volatile uint8_t *low_data_register 
	)
{
	adc_interface* interface = malloc(sizeof(adc_interface));

	interface->mux_selection_register = mux_selection_register;
	interface->control_a_register = control_a_register;
	interface->control_b_register = control_b_register;
	interface->high_data_register = high_data_register;
	interface->low_data_register = low_data_register;

	return interface;
}

void adc_setup(const adc_interface* interface)
{
	/* configure 1.1V as voltage reference, PB3 as input pin and left adjust the result */
	*(interface->mux_selection_register)  |= 0x03 | (1<<REFS1);
	*(interface->control_a_register)      |=  (1<<ADEN); /* enable ADC */
	*(interface->control_a_register)      &= ~(1<<ADIE); /* disable interrupt */
}

int adc_read(const adc_interface* interface)
{
	/* Start conversion */
	*(interface->control_a_register) |= (1<<ADSC);

	adc_wait_for_conversion(interface);
	
	int value = *(interface->low_data_register);
	value += (*(interface->high_data_register)<<8);

	return value;
}