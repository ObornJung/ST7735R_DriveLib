//
//  OBST7735R_DriveLib.c
//  ST7735R_DriveLib
//
//  Created by ObornJung on 11/24/14.
//  Copyright (c) 2014 JOMA Inc. All rights reserved.
//

#include <stdint.h>
#include <math.h>
#include "OBST7735R_Adapter.h"
#include "OBST7735R_DriveLib.h"

typedef enum {
    OBST7735R_IPF_12        = 0x03,
    OBST7735R_IPF_16        = 0x05,
    OBST7735R_IPF_18        = 0x06,
    OBST7735R_IPF_NOUSED    = 0x07
}OBST7735R_IPF;

OBRect OBRectMake(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    OBRect rect;
    rect.orignal.x = x;
    rect.orignal.y = y;
    rect.size.width = width;
    rect.size.height = height;
    return rect;
}

static const OBRect OBST7735R_SCREEN = {{0, 0}, {128, 128}};
static const OBSize OBST7735R_ASCIISIZE = {5, 8};

static const uint8_t OBASCII_NUMBER[] = {
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x72, 0x49, 0x49, 0x49, 0x46, // 2
    0x21, 0x41, 0x49, 0x4D, 0x33, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
    0x41, 0x21, 0x11, 0x09, 0x07, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x46, 0x49, 0x49, 0x29, 0x1E, // 9
};

//
//
#define OBST7735R_SWRESET       0x01    ///< software reset
#define OBST7735R_SLPIN         0x10    ///< sleep in
#define OBST7735R_SLOUT         0x11    ///< sleep out
//
// dispaly mode command
#define OBST7735R_NORON         0x13    ///< normal display mode on
#define OBST7735R_PTLON         0x12    ///< partial display mode on
#define OBST7735R_PTLAR         0x30    ///< partial area
//
// display inversion command
#define OBST7735R_INVOFF        0x20    ///< display inversion off
#define OBST7735R_INVON         0x21    ///< display inversion on

#define OBST7735R_DISPOFF       0x28    ///< display off
#define OBST7735R_DISPON        0x29    ///< display on

#define OBST7735R_CASET         0x2A    ///< column address set
#define OBST7735R_RASET         0x2B    ///< row address set
#define OBST7735R_RAMWR         0x2C    ///< memory write

#define OBST7735R_RGBSET        0x2D    ///< color setting for 4K,65k and 262k ?????

#define OBST7735R_MADCTL        0x36    ///< memory data access control
#define OBST7735R_COLMOD        0x3A    ///< interface pixel format

//
// panel function control command
#define OBST7735R_FRMCTR1       0xB1    ///< frame rate control(in normal mode/full colors)
#define OBST7735R_FRMCTR2       0xB2    ///< frame rate control(in idle mode/8-colors)
#define OBST7735R_FRMCTR3       0xB3    ///< frame rate control(in partial mode/full colors)
#define OBST7735R_INVCTR        0xB4    ///< display inversion control
#define OBST7735R_PWCTR1        0xC0    ///< power control 1
#define OBST7735R_PWCTR2        0xC1    ///< power control 2
#define OBST7735R_PWCTR3        0xC2    ///< power control 3
#define OBST7735R_PWCTR4        0xC3    ///< power control 4
#define OBST7735R_PWCTR5        0xC4    ///< power control 5
#define OBST7735R_VMCTR1        0xC5    ///< VCOM control 1
#define OBST7735R_VMOFCTR       0xC7    ///< VCOM offset control
#define OBST7735R_NVFCTR1       0xD9    ///< NVM control status
#define OBST7735R_NVFCTR3       0xDF    ///< NVM write command
#define OBST7735R_GMCTRP1       0xE0    ///< Gamma ('+'polarity) correction characteristics setting
#define OBST7735R_GMCTRN1       0xE1    ///< Gamma ('-'polarity) correction characteristics setting

extern void OBST7735R_enterSleep(void);
extern void OBST7735R_exitSleep(void);

#pragma mark - inner methods
static inline void OBST7735R_SWReset(void);
static inline void OBST7735R_write_data(uint8_t data);
static inline void OBST7735R_write_command(uint8_t cmd);
static inline void OBST7735R_setPixelFormat(OBST7735R_IPF pixelFormat);

static inline BOOL OBST7735R_setDrawWindow(OBRect rect);
static inline void OBST7735R_drawPixel(uint8_t x, uint8_t y, uint16_t color);
static inline void OBST7735R_writePixelData(const uint16_t * colorArray, uint32_t count);

#pragma mark - OBST7735R control function
void OBST7735R_init(void)
{
    //
    // software reset
    OBST7735R_SWReset();
    
    //
    // exit sleep mod
    OBST7735R_exitSleep();
    
    //
    // set interface pixel format:65k mode
    OBST7735R_setPixelFormat(OBST7735R_IPF_16);
    
    //
    // init diplay area:128x128
    OBST7735R_write_command(OBST7735R_CASET);
    OBST7735R_write_data(0x00); OBST7735R_write_data(0x00);
    OBST7735R_write_data(0x00); OBST7735R_write_data(0x7F);
    OBST7735R_write_command(OBST7735R_RASET);
    OBST7735R_write_data(0x00); OBST7735R_write_data(0x00);
    OBST7735R_write_data(0x00); OBST7735R_write_data(0x7F);

    //
    // init dispaly inversion control
    OBST7735R_write_command(OBST7735R_INVCTR);
    OBST7735R_write_data(0x07);

    //
    // init memory data access control:MX, MY, RGB mode
    OBST7735R_write_command(OBST7735R_MADCTL);
    OBST7735R_write_data(0x34);
    
    //
    // init frame rate
    OBST7735R_write_command(OBST7735R_FRMCTR1);
    OBST7735R_write_data(0x01); OBST7735R_write_data(0x2C); OBST7735R_write_data(0x2D);
    OBST7735R_write_command(OBST7735R_FRMCTR2);
    OBST7735R_write_data(0x01); OBST7735R_write_data(0x2C); OBST7735R_write_data(0x2D);
    OBST7735R_write_command(OBST7735R_FRMCTR3);
    OBST7735R_write_data(0x01); OBST7735R_write_data(0x2C); OBST7735R_write_data(0x2D);
    OBST7735R_write_data(0x01); OBST7735R_write_data(0x2C); OBST7735R_write_data(0x2D);
    
    //
    // init power sequence
    OBST7735R_write_command(OBST7735R_PWCTR1);
    OBST7735R_write_data(0xA2); OBST7735R_write_data(0x02); OBST7735R_write_data(0x84);
    OBST7735R_write_command(OBST7735R_PWCTR2);
    OBST7735R_write_data(0xC5);
    OBST7735R_write_command(OBST7735R_PWCTR3);
    OBST7735R_write_data(0x0A); OBST7735R_write_data(0x00);
    OBST7735R_write_command(OBST7735R_PWCTR4);
    OBST7735R_write_data(0x8A); OBST7735R_write_data(0x2A);
    OBST7735R_write_command(OBST7735R_PWCTR5);
    OBST7735R_write_data(0x8A); OBST7735R_write_data(0xEE);
    OBST7735R_write_command(OBST7735R_VMCTR1);  ///< VCOM
    OBST7735R_write_data(0X0E);
    
    //
    // gamma correction
    OBST7735R_write_command(OBST7735R_GMCTRP1);
    OBST7735R_write_data(0x0f); OBST7735R_write_data(0x1a);
    OBST7735R_write_data(0x0f); OBST7735R_write_data(0x18);
    OBST7735R_write_data(0x2f); OBST7735R_write_data(0x28);
    OBST7735R_write_data(0x20); OBST7735R_write_data(0x22);
    OBST7735R_write_data(0x1f); OBST7735R_write_data(0x1b);
    OBST7735R_write_data(0x23); OBST7735R_write_data(0x37);
    OBST7735R_write_data(0x00); OBST7735R_write_data(0x07);
    OBST7735R_write_data(0x02); OBST7735R_write_data(0x10);
    OBST7735R_write_command(OBST7735R_GMCTRN1);
    OBST7735R_write_data(0x0f); OBST7735R_write_data(0x1b);
    OBST7735R_write_data(0x0f); OBST7735R_write_data(0x17);
    OBST7735R_write_data(0x33); OBST7735R_write_data(0x2c);
    OBST7735R_write_data(0x29); OBST7735R_write_data(0x2e);
    OBST7735R_write_data(0x30); OBST7735R_write_data(0x30);
    OBST7735R_write_data(0x39); OBST7735R_write_data(0x3f);
    OBST7735R_write_data(0x00); OBST7735R_write_data(0x07);
    OBST7735R_write_data(0x03); OBST7735R_write_data(0x10);
    
    //
    // display on
    OBST7735R_write_command(OBST7735R_DISPON);
}

void OBST7735R_enterSleep(void)
{
    OBST7735R_write_command(OBST7735R_SLPIN);
    OBST7735R_DELAYMS(120);
}

void OBST7735R_exitSleep(void)
{
    OBST7735R_write_command(OBST7735R_SLOUT);
    OBST7735R_DELAYMS(120);
}

#pragma mark - drawing function
void OBST7735R_drawLine(OBPoint start, OBPoint end, uint16_t color)
{
    uint8_t x0 = start.x < end.x ? start.x : end.x;
    uint8_t y0 = start.y < end.y ? start.y : end.y;
    uint8_t x1 = start.x < end.x ? end.x : start.x;
    uint8_t y1 = start.y < end.y ? end.y : start.y;
    uint8_t length = 0;
    
    if (x0 == x1)
    {
        length = y1 - y0;
        if (length > 0)
        {
            OBST7735R_setDrawWindow(OBRectMake(x0, y0, length, 1));
            OBST7735R_write_command(OBST7735R_RAMWR);
            while (length --)
            {
                OBST7735R_write_data((uint8_t)(color >> 8));
                OBST7735R_write_data((uint8_t)color);
            }
        }
        
    }
    else if (y0 == y1)
    {
        length = x1 - x0;
        if (length > 0)
        {
            OBST7735R_setDrawWindow(OBRectMake(x0, y0, 1, length));
            OBST7735R_write_command(OBST7735R_RAMWR);
            while (length --)
            {
                OBST7735R_write_data((uint8_t)(color >> 8));
                OBST7735R_write_data((uint8_t)color);
            }
        }
    }
    else
    {
        uint8_t dx = x1 - x0;
        uint8_t dy = y1 - y0;
        if (dx > dy)
        {
            float scale = dy/dx;
            while (dx --)
            {
                OBST7735R_drawPixel(x0 + dx, y0 + dx * scale, color);
            }
        }
        else
        {
            float scale = dx/dy;
            while (dy --)
            {
                OBST7735R_drawPixel(x0 + dy * scale, y0 + dy, color);
            }
        }
    }
}

void OBST7735R_drawRect(OBRect frame, uint16_t color, BOOL fill)
{
    if (OBST7735R_setDrawWindow(frame))
    {
        if (fill)
        {
            uint32_t pixelCount = frame.size.width * frame.size.height;
            OBST7735R_write_command(OBST7735R_RAMWR);
            while (pixelCount --)
            {
                OBST7735R_write_data((uint8_t)(color >> 8));
                OBST7735R_write_data((uint8_t)color);
            }
        }
        else
        {
            OBPoint start = frame.orignal;
            OBPoint end;
            //  ___
            //
            end.x = frame.orignal.x + frame.size.width; end.y = frame.orignal.y;
            OBST7735R_drawLine(start, end, color);
            //  ___
            // |
            end.x = frame.orignal.x; end.y = frame.orignal.y + frame.size.height;
            OBST7735R_drawLine(start, end, color);
            //  ___
            // |___
            start.x = frame.orignal.x + frame.size.width; end.y = frame.orignal.y + frame.size.height;
            OBST7735R_drawLine(end, start, color);
            //  ___
            // |___|
            end.x = frame.orignal.x + frame.size.width; end.y = frame.orignal.y;
            OBST7735R_drawLine(start, end, color);
        }
    }
}

void OBST7735R_fillScreen(uint16_t color)
{
    OBST7735R_drawRect(OBRectMake(0, 0, OBST7735R_SCREEN.size.width, OBST7735R_SCREEN.size.height), color, TRUE);
}

void OBST7735R_drawBitmap(const uint16_t * image, OBRect frame)
{
    if (OBST7735R_setDrawWindow(frame))
    {
        OBST7735R_writePixelData(image, frame.size.width * frame.size.height);
    }
}

void OBST7735R_drawNumber(OBPoint startPoint, uint8_t number, uint16_t textColor, uint16_t bgColor, uint8_t fontSize)
{
    if (fontSize > 0)
    {
        OBRect numberFrame;
        numberFrame.orignal = startPoint;
        numberFrame.size.width = OBST7735R_ASCIISIZE.width * fontSize;
        numberFrame.size.height = OBST7735R_ASCIISIZE.height * fontSize;
        
        if (OBST7735R_setDrawWindow(numberFrame))
        {
            OBST7735R_write_command(OBST7735R_RAMWR);
            number = number % 10;
            for (int row = 0; row < OBST7735R_ASCIISIZE.height; row ++)
            {
                uint8_t bitMask = (1 << row);
                for (int i = 0; i < fontSize; i ++)
                {
                    for (int col = 0; col < OBST7735R_ASCIISIZE.width; col ++)
                    {
                        if (OBASCII_NUMBER[number * OBST7735R_ASCIISIZE.width + col] & bitMask)
                        {
                            for (int j = 0; j < fontSize; j ++)
                            {
                                OBST7735R_write_data((uint8_t)(textColor >> 8));
                                OBST7735R_write_data((uint8_t)textColor);
                            }
                        }
                    }
                }
            }
        }
    }
}

#pragma mark - private methods
void OBST7735R_SWReset(void)
{
    OBST7735R_write_command(OBST7735R_SWRESET);
    OBST7735R_DELAYMS(120);
}

void OBST7735R_setPixelFormat(OBST7735R_IPF pixelFormat)
{
    OBST7735R_write_command(OBST7735R_COLMOD);
    OBST7735R_write_data(pixelFormat);
}

static inline BOOL OBST7735R_setDrawWindow(OBRect rect)
{
    uint8_t x0 = rect.orignal.x + OBST7735R_SCREEN.orignal.x;
    uint8_t x1 = rect.orignal.x + rect.size.width;
    uint8_t y0 = rect.orignal.y + OBST7735R_SCREEN.orignal.y;
    uint8_t y1 = rect.orignal.y + rect.size.height;
    if (x0 < OBST7735R_SCREEN.size.width && x1 < OBST7735R_SCREEN.size.width
        && y0 < OBST7735R_SCREEN.size.height && y1 < OBST7735R_SCREEN.size.height)
    {
        //
        // column address set
        OBST7735R_write_command(OBST7735R_CASET);
        OBST7735R_write_data(0x00); OBST7735R_write_data(x0);
        OBST7735R_write_data(0x00); OBST7735R_write_data(x1);
        //
        // row address set
        OBST7735R_write_command(OBST7735R_RASET);
        OBST7735R_write_data(0x00); OBST7735R_write_data(y0);
        OBST7735R_write_data(0x00); OBST7735R_write_data(y1);
        return FALSE;
    }
    return TRUE;
}

static inline void OBST7735R_writePixelData(const uint16_t * colorArray, uint32_t count)
{
    OBST7735R_write_command(OBST7735R_RAMWR);
    for (int i = 0; i < count; i ++)
    {
        uint16_t color = colorArray[i];
        OBST7735R_write_data((uint8_t)(color >> 8));
        OBST7735R_write_data((uint8_t)color);
    }
}

static inline void OBST7735R_drawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    if (OBST7735R_setDrawWindow(OBRectMake(x, y, 1, 1)))
    {
        OBST7735R_writePixelData(&color, 1);
    }
}

static inline void OBST7735R_write_command(uint8_t cmd)
{
    
}

static inline void OBST7735R_write_data(uint8_t data)
{
    
}
