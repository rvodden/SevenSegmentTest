/*
 * ioport.h
 *
 * Created: 11/11/2017 08:39:48
 *  Author: Richard
 */ 


#ifndef IOPORT_H_
#define IOPORT_H_

#include <stdint.h>

#ifdef IOPORT_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif /* TWO_WIRE_IMPORT */

/* Constants */

/* Types */

typedef struct ioport_interface ioport_interface;

/* Global Variables */

/* Function Prototypes */

EXTERN ioport_interface* ioport_construct ( volatile uint8_t* direction_register, volatile uint8_t* port, volatile uint8_t* pin, uint8_t  bit );

EXTERN void ioport_setlow(ioport_interface* interface);
EXTERN void ioport_sethigh(ioport_interface* interface);
EXTERN void ioport_toggle(ioport_interface* interface);

EXTERN void ioport_pause_until_high(ioport_interface* interface);

EXTERN void ioport_configure_as_input(ioport_interface* interface);
EXTERN void ioport_configure_as_output(ioport_interface* interface);

#undef IOPORT_IMPORT
#undef EXTERN

#endif /* IOPORT_H_ */