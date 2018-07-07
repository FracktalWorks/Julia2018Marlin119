#ifndef _FRACKTAL_WORKS_H
#define _FRACKTAL_WORKS_H

#define JULIA_2018_GLCD       0
#define JULIA_2018_GLCD_HB    1
#define JULIA_2018_RPI        2
#define JULIA_2018_RPI_E      3

#define VARIANT JULIA_2018_GLCD

#ifndef VARIANT
	#error "Variant not defined"
#endif

#define BV(var) (VARIANT == var)

#endif // _FRACKTAL_WORKS_H
