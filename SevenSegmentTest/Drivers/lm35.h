/*
 * lm35.h
 *
 * Created: 11/11/2017 11:49:49
 *  Author: LONRV1
 */ 


#ifndef LM35_H_
#define LM35_H_

#include "temperature.h"
#include "adc.h"

#ifdef LM35_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Constants */

/* Types */

/* Global Variables */

/* Function Prototypes */

ts_instance* lm35_ts_construct(adc_interface* interface);

#undef LM35_IMPORT
#undef EXTERN

#endif /* LM35_H_ */