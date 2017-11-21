/*
 * max7221.h
 *
 * Created: 07/11/2017 15:31:01
 *  Author: Richard
 */ 


#ifndef MAX7221_H_
#define MAX7221_H_

#include "7segment.h"

#ifdef MAX7221_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Constants */

/* Types */

/* Global Variables */

/* Function Prototypes */

EXTERN ss_instance* max7221_ss_construct(const spi_interface *interface, int digits);

#undef MAX7221_import
#undef EXTERN

#endif /* MAX7221_H_ */