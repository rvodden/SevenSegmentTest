/*
 * temperature.h
 *
 * Created: 11/11/2017 10:57:10
 *  Author: Richard
 */ 


#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#ifdef TEMPERATURE_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Types */

typedef struct ts_instance ts_instance;

/* Function Prototypes */

int ts_get_temperature(ts_instance*);

#undef TEMPERATURE_IMPORT
#undef EXTERN

#endif /* TEMPERATURE_H_ */