/*
 * adc.c
 *
 * Created: 10/11/2017 17:43:30
 *  Author: Richard
 */ 

#include <avr/io.h>

#include "7segment.h"

#define ADC_IMPORT
#include "adc.h"

/* Private Macros and Constants */

/* Declarations of opaque structures */

/* Private Functions */



void adc_setup(const adc_interface* interface)
{
	/* configure 1.1V as voltage reference, PB3 as input pin and left adjust the result */
	ADMUX  |= 0x03 | (1<<REFS1);
	ADCSRA |=  (1<<ADEN); /* enable ADC */
	ADCSRA &= ~(1<<ADIE); /* disable interrupt */
}

/* Implementation of public functions */

int adc_read(const adc_interface* interface)
{
	/* Start conversion */
	ADCSRA |= (1<<ADSC);

	/* wait for conversion to complete */
	while (ADCSRA & (1<<ADSC));

	int value = ADCL;
	value += (ADCH<<8);

	return value;
}