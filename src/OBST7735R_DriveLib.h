/**
 Filename:       OBST7735R_DriveLib.h
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $

 Description:    This file contains the ST7735R driver.

 Copyright 2015 ObornJung. All rights reserved.
 */

#ifndef __H__JMEST7735R_DriveLib__H__
#define __H__JMEST7735R_DriveLib__H__

/*********************************************************************
 * INCLUDES
 */
#include "JMEBase.h"
#include "JMEGeometry.h"

/*********************************************************************
 * TYPEDEFS
 */
typedef struct {
    JMERect             iconFrame;
    JMESize             iconSize;
    const uint8_t       * iconData;
    uint16_t            color;
    uint16_t            bgColor;
}JMEMenuIcon_t;

/*********************************************************************
 * MACROS
 */
#define JMEST7735RSCREENWIDTH           128
#define JMEST7735RSCREENHEIGHT          160

/*********************************************************************
 * EXTERN VARIABLES
 */
JME_EXTERN const JMERect kJMEST7735RScreenFrame;

/*********************************************************************
 * FUNCTIONS
 */
//
// JMEST7735R control function
JME_EXTERN void JMEST7735R_init(void);
JME_EXTERN void JMEST7735R_enterSleep(void);
JME_EXTERN void JMEST7735R_exitSleep(void);
//
// drawing function
JME_EXTERN void JMEST7735R_refreshScreen(void);
JME_EXTERN void JMEST7735R_fillScreen(uint16_t color);
JME_EXTERN void JMEST7735R_drawLine(JMEPoint start, JMEPoint end, uint16_t color);
JME_EXTERN void JMEST7735R_drawRect(JMERect frame, uint16_t color, BOOL fill);
JME_EXTERN void JMEST7735R_drawBitmap(const uint16_t * image, JMERect frame, BOOL isHighLight);
JME_EXTERN void JMEST7735R_drawBinaryImage(const uint8_t * image, JMERect frame,
                                           uint16_t fgColor, uint16_t bgColor);
JME_EXTERN void JMEST7735R_drawBinaryIcon(const JMEMenuIcon_t * icon);
JME_EXTERN void JMEST7735R_drawNumber(JMEPoint startPoint, uint16_t number,
                                      uint16_t textColor, uint16_t bgColor, uint8_t fontSize);
JME_EXTERN void JMEST7735R_drawMenuIcon(const JMEMenuIcon_t * icon, BOOL isHighLight);
JME_EXTERN void JMEST7735R_drawString(JMEPoint startPoint, const char * string,
                                      uint16_t textColor, uint16_t bgColor, uint8_t fontSize);

#endif /* defined(__H__JMEST7735R_DriveLib__H__) */
