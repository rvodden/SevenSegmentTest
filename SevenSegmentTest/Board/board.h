/*
 * board.h
 *
 * Created: 07/11/2017 15:13:42
 *  Author: Richard
 */ 


#ifndef BOARD_H_
#define BOARD_H_

#ifdef BOARD_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif /* BOARD_IMPORT */

#define F_CPU 1000000UL

#define LED PB1
#define TS  PB3


EXTERN void board_init();

#undef BOARD_IMPORT
#undef EXTERN

#endif /* BOARD_H_ */