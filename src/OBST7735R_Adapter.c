/**
 Filename:       OBST7735R_Adapter.c
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $
 
 Description:    This file support hardware interface for the ST7735R driver.
 
 Copyright 2015 JONMA Inc. All rights reserved.
 */

#include "JMEST7735R_Adapter.h"
#include "JMEST7735R_DriveLib.h"

void JMEST7735R_portInit(void)
{
	//
	// 端口初始化
    JMEST7735R_LEDOFF();
}

void JMEST7735R_IOEnterSleep(BOOL isSleep) {
    if (isSleep) {
        
    } else {
        
    }
}
