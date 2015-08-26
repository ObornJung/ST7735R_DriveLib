/**
 Filename:       JMEMath.h
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $
 Author:         Oborn.Jung

 Description:    This file contains the math functions for embed platform.

 Copyright 2015 JONMA Inc. All rights reserved.
 */

#ifndef __H__JMEMath__H__
#define __H__JMEMath__H__

#include <math.h>
#include "JMEBase.h"

#define JMEMin(a, b)                    ((a) < (b) ? (a) : (b))
#define JMEMax(a, b)                    ((a) > (b) ? (a) : (b))
#define JMEBit(n)                       (1 << (n))
#define JMEABS(a)                       ((a) > 0 ? (a) : -(a))
#define JMEMod(n, mod)                  do {\
while ((n) < (0)) {\
    (n) += (mod);\
}\
while ((n) >= (mod)) {\
    (n) -= (mod);\
}\
}while(0)

#endif /* defined(__H__JMEMath__H__) */
