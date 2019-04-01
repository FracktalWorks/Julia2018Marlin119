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

#define JULIA_2018_GLCD                   0
#define JULIA_2018_GLCD_HB                1
#define JULIA_2018_RPI                    2
#define JULIA_2018_RPI_E                  3
#define JULIA_2018_PRO_SINGLE             4
#define JULIA_2018_PRO_DUAL               5
#define JULIA_2018_PRO_SINGLE_A           6
#define JULIA_2018_PRO_DUAL_A             7

#include "_Variant.h"

#define BV(var)       (VARIANT == var)
#define NBV(var)      (VARIANT != var)

#define BV_NPI()            ( BV(JULIA_2018_GLCD) || BV(JULIA_2018_GLCD_HB) )
#define BV_PIX()            ( BV(JULIA_2018_RPI) || BV(JULIA_2018_RPI_E) )
#define BV_REG()            ( BV_NPI() || BV(JULIA_2018_RPI) )
#define BV_PRO()            ( BV(JULIA_2018_PRO_SINGLE) || BV(JULIA_2018_PRO_DUAL) )
// #define BV_PI()          ( BV_PIX() || BV_PRO() )
#define BV_PRO_ABL()        ( BV(JULIA_2018_PRO_SINGLE_A) || BV(JULIA_2018_PRO_DUAL_A) )
#define BV_PRO_SINGLE()     ( BV(JULIA_2018_PRO_SINGLE) || BV(JULIA_2018_PRO_SINGLE_A) )
#define BV_PRO_DUAL()       ( BV(JULIA_2018_PRO_DUAL) || BV(JULIA_2018_PRO_DUAL_A) )

#endif // _FRACKTAL_WORKS_H
