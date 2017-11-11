/*
 * spi.h
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

typedef struct spi_interface spi_interface;

/* Global Variables */

/* Function Prototypes */

EXTERN void spi_setup(const spi_interface* interface);
EXTERN void spi_write(const spi_interface* interface, uint8_t addr, uint8_t data);

#undef SPI_IMPORT
#undef EXTERN
#endif /* SPI_H_ */