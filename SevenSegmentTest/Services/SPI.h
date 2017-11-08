/*
 * SPI.h
 *
 * @copyright 2017 Richard Vodden
 * @license GPL 3.0
 * @author Richard Vodden <richard@vodden.com>
 * @file
 */ 


#ifndef SPI_H_
#define SPI_H_

#ifdef SPI_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif /* SPI_IMPORT */

/* Constants */

/* Types */

typedef struct SPI_interface SPI_interface;

/* Global Variables */

/* Function Prototypes */

EXTERN void SPI_Setup(const SPI_interface* interface);
EXTERN void SPI_Write(const SPI_interface* interface, uint8_t addr, uint8_t data);

#undef SPI_IMPORT
#undef EXTERN
#endif /* SPI_H_ */