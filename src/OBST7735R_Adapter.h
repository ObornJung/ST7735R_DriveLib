/**
 Filename:       OBST7735R_Adapter.h
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $
 
 Description:    This file support hardware interface for the ST7735R driver.
 
 Copyright 2015 JONMA Inc. All rights reserved.
 */

#ifndef __H__JMEST7735R_Adapter_H__
#define __H__JMEST7735R_Adapter_H__

#include <ioCC2540.h>
#include "JMEBase.h"
#include "JMESystem.h"
#include "hal_types.h"

#define JMEST7735R_RESETENABLE()        	      	///< reset
#define JMEST7735R_RESETDISABLE()       	
#define JMEST7735R_CDSET()              	      	///< data/command selection.’1’:data or parameter,'0':command.
#define JMEST7735R_CDCLR()              	
#define JMEST7735R_RWSET()              	      	///< Write selection
#define JMEST7735R_RWCLR()              	
#define JMEST7735R_RDSET()                     		///< Read selection
#define JMEST7735R_RDCLR()
#define JMEST7735R_CSSET()              	      	///< Chip selection
#define JMEST7735R_CSCLR()              	
#define JMEST7735R_LEDON()              	
#define JMEST7735R_LEDOFF()             	
#define JMEST7735R_delayMS(n)           	
#define JMEST7735R_writeByte(byte)	
#define JMEST7735R_readByte()	

#define JMEST7735R_NOP()                ASM_NOP

JME_EXTERN void JMEST7735R_portInit(void);
JME_EXTERN void JMEST7735R_IOEnterSleep(BOOL isSleep);

#endif /* __H__JMEST7735R_Adapter_H__ */
