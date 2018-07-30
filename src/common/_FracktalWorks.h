#ifndef _FRACKTAL_WORKS_H
#define _FRACKTAL_WORKS_H

/** Classifications 
 * 
 * NPI: GLCD, GLCD_HB
 * REG: GLCD, GLCD_HB, RPI
 * PIX: RPI, RPI_E
 * PI: RPI, RPI_E, PRO_SINGLE, PRO_DUAL
 * PRO: PRO_SINGLE, PRO_DUAL
 * 
**/ 

#define JULIA_2018_GLCD					0
#define JULIA_2018_GLCD_HB			1
#define JULIA_2018_RPI					2
#define JULIA_2018_RPI_E				3
#define JULIA_2018_PRO_SINGLE		4
#define JULIA_2018_PRO_DUAL			5

#include "_Variant.h"

#define BV(var)   (VARIANT == var)
#define NBV(var)  (VARIANT != var)

#define BV_NPI()	( BV(JULIA_2018_GLCD) || BV(JULIA_2018_GLCD_HB) )
#define BV_PIX()	( BV(JULIA_2018_RPI) || BV(JULIA_2018_RPI_E) )
#define BV_REG()	( BV(JULIA_2018_GLCD) || BV(JULIA_2018_GLCD_HB) || BV(JULIA_2018_RPI) )
#define BV_PRO()	( BV(JULIA_2018_PRO_SINGLE) || BV(JULIA_2018_PRO_DUAL) )

#endif // _FRACKTAL_WORKS_H
