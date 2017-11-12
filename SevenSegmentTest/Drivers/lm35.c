/*
 * lm35.h
 *
 * Created: 11/11/2017 12:39:06
 *  Author: LONRV1
 */ 

#include <stdlib.h>

#include <adc.h>

#define TEMPERATURE_IMPORT
#include "temperature.h"

#define LM35_IMPORT
#include "lm35.h"

/* Private Macros and Constants */

#define CYCLE_LENGTH 10

/* Declarations of opaque structures */

struct ts_instance {
	int* values;
	char   cycle_number;
	adc_interface* adc;
};

/* Private Functions */

/* Implementation of public functions */

ts_instance* lm35_ts_construct(adc_interface *interface) {
	ts_instance *instance = malloc(sizeof(ts_instance));
	instance->values = malloc(sizeof(int)*CYCLE_LENGTH);
	instance->values[0] = 0;
	instance->values[1] = 0;
	instance->values[2] = 0;
	instance->cycle_number = 0;
	instance->adc = interface;
	return instance;
}

int ts_get_temperature(ts_instance* instance) {
	instance->values[(int)instance->cycle_number] = adc_read(instance->adc) * 1.08;
	instance->cycle_number = ( instance->cycle_number + 1 ) % CYCLE_LENGTH;
	int retval = 0;
	for (int i = 0; i < CYCLE_LENGTH; i++) {
		retval += instance->values[i];
	}
	retval /= CYCLE_LENGTH;
	return retval;
}