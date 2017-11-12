/*
 * adc.h
 *
 * Created: 10/11/2017 16:54:28
 *  Author: Richard
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#ifdef ADC_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif /* adc_IMPORT */

/* Constants */

/* Types */

typedef struct adc_interface adc_interface;

/* Global Variables */

/* Function Prototypes */

EXTERN adc_interface* adc_construct(
	volatile uint8_t *mux_selection_register,
	volatile uint8_t *control_a_register,
	volatile uint8_t *control_b_register,
	volatile uint8_t *high_data_register,
	volatile uint8_t *low_data_register
);

EXTERN void adc_setup(const adc_interface* interface);
EXTERN int adc_read(const adc_interface* interface);

#undef ADC_IMPORT
#undef EXTERN

#endif /* ADC_H_ */