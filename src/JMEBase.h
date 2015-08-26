/**
 Filename:       JMEBase.h
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $
 Author:         Oborn.Jung

 Description:    This file contains the base functions and defines for embed platform.

 Copyright 2015 JONMA Inc. All rights reserved.
 */

#ifndef __H__JMEBase__H__
#define __H__JMEBase__H__

//#include <stdint.h>

#ifdef __cplusplus
#define JME_EXTERN extern "C"
#else
#define JME_EXTERN extern
#endif

/*#ifndef BOOL
#ifndef bool
typedef enum {
    FALSE   = 0,
    TRUE    = 1
}BOOL;
typedef BOOL bool;
#endif
#endif*/

typedef	signed char	    int8_t;
typedef unsigned char   uint8_t;
typedef signed short    int16_t;
typedef unsigned short  uint16_t;
typedef signed long     int32_t;
typedef unsigned long   uint32_t;
typedef uint8_t BOOL;

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    !FALSE
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /* defined(__H__JMEBase__H__) */
