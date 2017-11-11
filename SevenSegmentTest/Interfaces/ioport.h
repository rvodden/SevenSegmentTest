/*
 * ioport.h
 *
 * Created: 11/11/2017 08:39:48
 *  Author: Richard
 */ 


#ifndef IOPORT_H_
#define IOPORT_H_

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

#undef IOPORT_IMPORT
#undef EXTERN

#endif /* IOPORT_H_ */