//
//  OBST7735R_DriveLib.h
//  ST7735R_DriveLib
//
//  Created by ObornJung on 11/24/14.
//  Copyright (c) 2014 JOMA Inc. All rights reserved.
//

#ifndef __ST7735R_DriveLib__OBST7735R_DriveLib__
#define __ST7735R_DriveLib__OBST7735R_DriveLib__

#include <stdio.h>

typedef uint8_t BOOL;

#define FALSE   0
#define TRUE    (!FALSE)

typedef struct
{
    uint8_t x;
    uint8_t y;
}OBPoint;

typedef struct
{
    uint8_t width;
    uint8_t height;
}OBSize;

typedef struct
{
    OBPoint orignal;
    OBSize size;
}OBRect;

//
// OBST7735R control function
extern void OBST7735R_init(void);
extern void OBST7735R_enterSleep(void);
extern void OBST7735R_exitSleep(void);
//
// drawing function
extern void OBST7735R_fillScreen(uint16_t color);
extern void OBST7735R_drawLine(OBPoint start, OBPoint end, uint16_t color);
extern void OBST7735R_drawRect(OBRect frame, uint16_t color, BOOL fill);
extern void OBST7735R_drawBitmap(const uint16_t * image, OBRect frame);
extern void OBST7735R_drawNumber(OBPoint startPoint, uint8_t number, uint16_t textColor, uint16_t bgColor, uint8_t fontSize);

#endif /* defined(__ST7735R_DriveLib__OBST7735R_DriveLib__) */
