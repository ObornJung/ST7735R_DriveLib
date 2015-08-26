/**
 Filename:       OBST7735R_DriveLib.c
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $

 Description:    This file contains the ST7735R driver.

 Copyright 2015 ObornJung. All rights reserved.
 */

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include "JMEBase.h"
#include "JMEMath.h"
#include "JMERemoterRes.h"
#include "JMEST7735R_Adapter.h"
#include "JMEST7735R_DriveLib.h"

/*********************************************************************
 * TYPEDEFS
 */
typedef enum {
    JMEST7735R_IPF_12        = 0x03,
    JMEST7735R_IPF_16        = 0x05,
    JMEST7735R_IPF_18        = 0x06,
    JMEST7735R_IPF_NOUSED    = 0x07
}JMEST7735R_IPF;

/*********************************************************************
 * CONSTANTS
 */
const JMERect kJMEST7735RScreenFrame = {{0, 0}, {JMEST7735RSCREENWIDTH, JMEST7735RSCREENHEIGHT}};
static const JMESize JMEST7735R_NUMBERSIZE = {5, 8};
static const JMESize JMEST7735R_ASCIISIZE = {8, 12};

static const uint8_t JMEASCII_NUMBER[] = {
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

/*********************************************************************
 * MACROS
 */
//
// run mode control command
#define JMEST7735R_SWReset       0x01    ///< software reset
#define JMEST7735R_SLPIN         0x10    ///< sleep in
#define JMEST7735R_SLOUT         0x11    ///< sleep out
//
// dispaly mode command
#define JMEST7735R_NORON         0x13    ///< normal display mode on
#define JMEST7735R_PTLON         0x12    ///< partial display mode on
#define JMEST7735R_PTLAR         0x30    ///< partial area
//
// display inversion command
#define JMEST7735R_INVOFF        0x20    ///< display inversion off
#define JMEST7735R_INVON         0x21    ///< display inversion on

#define JMEST7735R_DISPOFF       0x28    ///< display off
#define JMEST7735R_DISPON        0x29    ///< display on

#define JMEST7735R_CASET         0x2A    ///< column address set
#define JMEST7735R_RASET         0x2B    ///< row address set
#define JMEST7735R_RAMWR         0x2C    ///< memory write

#define JMEST7735R_RGBSET        0x2D    ///< color setting for 4K,65k and 262k ?????

#define JMEST7735R_MADCTL        0x36    ///< memory data access control
#define JMEST7735R_COLMOD        0x3A    ///< interface pixel format
//
// panel function control command
#define JMEST7735R_FRMCTR1       0xB1    ///< frame rate control(in normal mode/full colors)
#define JMEST7735R_FRMCTR2       0xB2    ///< frame rate control(in idle mode/8-colors)
#define JMEST7735R_FRMCTR3       0xB3    ///< frame rate control(in partial mode/full colors)
#define JMEST7735R_INVCTR        0xB4    ///< display inversion control
#define JMEST7735R_PWCTR1        0xC0    ///< power control 1
#define JMEST7735R_PWCTR2        0xC1    ///< power control 2
#define JMEST7735R_PWCTR3        0xC2    ///< power control 3
#define JMEST7735R_PWCTR4        0xC3    ///< power control 4
#define JMEST7735R_PWCTR5        0xC4    ///< power control 5
#define JMEST7735R_VMCTR1        0xC5    ///< VCOM control 1
#define JMEST7735R_VMOFCTR       0xC7    ///< VCOM offset control
#define JMEST7735R_NVFCTR1       0xD9    ///< NVM control status
#define JMEST7735R_NVFCTR3       0xDF    ///< NVM write command
#define JMEST7735R_GMCTRP1       0xE0    ///< Gamma ('+'polarity) correction characteristics setting
#define JMEST7735R_GMCTRN1       0xE1    ///< Gamma ('-'polarity) correction characteristics setting

#define JMEST7735R_EXTCTRL       0xF0    ///< Extension Command Control

#define JMEST7735R_seqWrite(byte)  JMEST7735R_RWCLR(); JMEST7735R_writeByte((byte)); JMEST7735R_RWSET();

/*********************************************************************
 * LOCAL FUNCTIONS
 */
#pragma mark - inner methods
static inline void _JMEST7735R_HDReset(void);
static inline void _JMEST7735R_SWReset(void);
static inline uint8_t _JMEST7735R_read_data(void);
static inline void _JMEST7735R_write_data(uint8_t data);
static inline void _JMEST7735R_write_command(uint8_t cmd);
static inline void _JMEST7735R_setPixelFormat(JMEST7735R_IPF pixelFormat);
static inline BOOL _JMEST7735R_setDrawWindow(JMERect rect);
static inline void _JJMEST7735R_drawPixel(uint8_t x, uint8_t y, uint16_t color);
static inline void _JMEST7735R_writePixelData(const uint16_t * colorArray, uint16_t count);

/*********************************************************************
 * IMPLEMENT OF PUBLIC FUNCTIONS
 */
#pragma mark - JMEST7735R control function
void JMEST7735R_init(void)
{
    //
    // device port init
    JMEST7735R_portInit();
    //
    // hardware reset
    _JMEST7735R_HDReset();
    //    //
    //    // software reset
    //    _JMEST7735R_SWReset();
    //
    // init frame rate
    _JMEST7735R_write_command(JMEST7735R_FRMCTR1);
    _JMEST7735R_write_data(0x02); _JMEST7735R_write_data(0x35); _JMEST7735R_write_data(0x36);
    _JMEST7735R_write_command(JMEST7735R_FRMCTR2);
    _JMEST7735R_write_data(0x02); _JMEST7735R_write_data(0x35); _JMEST7735R_write_data(0x36);
    _JMEST7735R_write_command(JMEST7735R_FRMCTR3);
    _JMEST7735R_write_data(0x02); _JMEST7735R_write_data(0x35); _JMEST7735R_write_data(0x36);
    _JMEST7735R_write_data(0x02); _JMEST7735R_write_data(0x35); _JMEST7735R_write_data(0x36);
    //
    // init dispaly inversion control
    _JMEST7735R_write_command(JMEST7735R_INVCTR);
    _JMEST7735R_write_data(0x03);
    //
    // init power sequence
    _JMEST7735R_write_command(JMEST7735R_PWCTR1);
    _JMEST7735R_write_data(0xA2); _JMEST7735R_write_data(0x02); _JMEST7735R_write_data(0x84);
    _JMEST7735R_write_command(JMEST7735R_PWCTR2);
    _JMEST7735R_write_data(0xC5);
    _JMEST7735R_write_command(JMEST7735R_PWCTR3);
    _JMEST7735R_write_data(0x0D); _JMEST7735R_write_data(0x00);
    _JMEST7735R_write_command(JMEST7735R_PWCTR4);
    _JMEST7735R_write_data(0x8A); _JMEST7735R_write_data(0x2A);
    _JMEST7735R_write_command(JMEST7735R_PWCTR5);
    _JMEST7735R_write_data(0x8A); _JMEST7735R_write_data(0xEE);
    _JMEST7735R_write_command(JMEST7735R_VMCTR1);  ///< VCOM
    _JMEST7735R_write_data(0X03);
    //
    // gamma correction
    _JMEST7735R_write_command(JMEST7735R_GMCTRP1);
    _JMEST7735R_write_data(0x12); _JMEST7735R_write_data(0x1c);
    _JMEST7735R_write_data(0x10); _JMEST7735R_write_data(0x18);
    _JMEST7735R_write_data(0x33); _JMEST7735R_write_data(0x2C);
    _JMEST7735R_write_data(0x25); _JMEST7735R_write_data(0x28);
    _JMEST7735R_write_data(0x28); _JMEST7735R_write_data(0x27);
    _JMEST7735R_write_data(0x2f); _JMEST7735R_write_data(0x3C);
    _JMEST7735R_write_data(0x00); _JMEST7735R_write_data(0x03);
    _JMEST7735R_write_data(0x03); _JMEST7735R_write_data(0x10);
    _JMEST7735R_write_command(JMEST7735R_GMCTRN1);
    _JMEST7735R_write_data(0x12); _JMEST7735R_write_data(0x1d);
    _JMEST7735R_write_data(0x10); _JMEST7735R_write_data(0x18);
    _JMEST7735R_write_data(0x2d); _JMEST7735R_write_data(0x28);
    _JMEST7735R_write_data(0x23); _JMEST7735R_write_data(0x28);
    _JMEST7735R_write_data(0x28); _JMEST7735R_write_data(0x26);
    _JMEST7735R_write_data(0x2f); _JMEST7735R_write_data(0x3B);
    _JMEST7735R_write_data(0x00); _JMEST7735R_write_data(0x03);
    _JMEST7735R_write_data(0x03); _JMEST7735R_write_data(0x10);
    //
    // init memory data access control:MX, MY, RGB mode
    _JMEST7735R_write_command(JMEST7735R_MADCTL);
    _JMEST7735R_write_data(0xC8);
    //
    // set interface pixel format:65k mode
    _JMEST7735R_setPixelFormat(JMEST7735R_IPF_16);
    //
    // display on
    _JMEST7735R_write_command(JMEST7735R_DISPON);

    _JMEST7735R_write_command(JMEST7735R_EXTCTRL);
    _JMEST7735R_write_data(0x01);
    _JMEST7735R_write_command(0xF6);
    _JMEST7735R_write_data(0x00);
}

void JMEST7735R_enterSleep(void)
{
    JMEST7735R_LEDOFF();
    _JMEST7735R_write_command(JMEST7735R_SLPIN);
    JMEST7735R_delayMS(20);
    JMEST7735R_IOEnterSleep(TRUE);
}

void JMEST7735R_exitSleep(void)
{
    JMEST7735R_IOEnterSleep(FALSE);
    JMEST7735R_LEDON();
    _JMEST7735R_write_command(JMEST7735R_SLOUT);
    JMEST7735R_delayMS(20);
}

#pragma mark - drawing function
void JMEST7735R_refreshScreen(void)
{
    _JMEST7735R_write_command(0x00);
}

void JMEST7735R_drawLine(JMEPoint start, JMEPoint end, uint16_t color)
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
            _JMEST7735R_setDrawWindow(JMERectMake(x0, y0, 1, length));
            _JMEST7735R_write_command(JMEST7735R_RAMWR);
            JMEST7735R_CDSET();
            JMEST7735R_RDSET();
            JMEST7735R_CSCLR();
            while (length --)
            {
                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                JMEST7735R_seqWrite((uint8_t)color);
            }
            JMEST7735R_CSSET();
        }
    }
    else if (y0 == y1)
    {
        length = x1 - x0;
        if (length > 0)
        {
            _JMEST7735R_setDrawWindow(JMERectMake(x0, y0, length, 1));
            _JMEST7735R_write_command(JMEST7735R_RAMWR);
            JMEST7735R_CDSET();
            JMEST7735R_RDSET();
            JMEST7735R_CSCLR();
            while (length --)
            {
                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                JMEST7735R_seqWrite((uint8_t)color);
            }
            JMEST7735R_CSSET();
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
                _JJMEST7735R_drawPixel(x0 + dx, y0 + dx * scale, color);
            }
        }
        else
        {
            float scale = dx/dy;
            while (dy --)
            {
                _JJMEST7735R_drawPixel(x0 + dy * scale, y0 + dy, color);
            }
        }
    }
}

void JMEST7735R_drawRect(JMERect frame, uint16_t color, BOOL fill) {
    if (_JMEST7735R_setDrawWindow(frame)) {
        if (fill) {
            uint16_t pixelCount = frame.size.width * frame.size.height;
            _JMEST7735R_write_command(JMEST7735R_RAMWR);
            JMEST7735R_CDSET();
            JMEST7735R_RDSET();
            JMEST7735R_CSCLR();
            while (pixelCount --)
            {
                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                JMEST7735R_seqWrite((uint8_t)color);
            }
            JMEST7735R_CSSET();
        } else {
            JMEPoint start = frame.origin;
            JMEPoint end;
            //  ___
            //
            end.x = frame.origin.x + frame.size.width; end.y = frame.origin.y;
            JMEST7735R_drawLine(start, end, color);
            //  ___
            //     |
            end.x -= 1;
            start.x = end.x; start.y = frame.origin.y + frame.size.height;
            JMEST7735R_drawLine(end, start, color);
            //  ___
            //  ___|
            start.x += 1; start.y -= 1;
            end.x = frame.origin.x; end.y = start.y;
            JMEST7735R_drawLine(end, start, color);
            //  ___
            // |___|
            start = frame.origin; end.y += 1;
            JMEST7735R_drawLine(start, end, color);
        }
    }
}

void JMEST7735R_fillScreen(uint16_t color)
{
    JMEST7735R_drawRect(JMERectMake(0, 0, kJMEST7735RScreenFrame.size.width, kJMEST7735RScreenFrame.size.height), color, TRUE);
}

void JMEST7735R_drawBitmap(const uint16_t * image, JMERect frame, BOOL isHighLight) {
    if (NULL != image) {
        if (_JMEST7735R_setDrawWindow(frame)) {
            _JMEST7735R_writePixelData(image, frame.size.width * frame.size.height);
        }
    }
}

void JMEST7735R_drawBinaryImage(const uint8_t * image, JMERect frame, uint16_t fgColor, uint16_t bgColor) {
    if (_JMEST7735R_setDrawWindow(frame)) {
        uint16_t pixelCount = frame.size.width * frame.size.height >> 3;
        _JMEST7735R_write_command(JMEST7735R_RAMWR);
        JMEST7735R_CDSET();
        JMEST7735R_RDSET();
        JMEST7735R_CSCLR();
        for (uint16_t n = 0; n < pixelCount; n ++) {
            uint8_t imageData = image[n];
//            for (int8_t i = 7; i > -1; i --) {
//                if (imageData & JMEBit(i)) {
//                    JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
//                    JMEST7735R_seqWrite((uint8_t)fgColor);
//                } else {
//                    JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
//                    JMEST7735R_seqWrite((uint8_t)bgColor);
//                }
//            }
            if (imageData & JMEBit(7)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(6)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(5)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(4)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(3)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(2)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(1)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
            if (imageData & JMEBit(0)) {
                JMEST7735R_seqWrite((uint8_t)(fgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)fgColor);
            } else {
                JMEST7735R_seqWrite((uint8_t)(bgColor >> 8));
                JMEST7735R_seqWrite((uint8_t)bgColor);
            }
        }
        JMEST7735R_CSSET();
    }
}

void JMEST7735R_drawBinaryIcon(const JMEMenuIcon_t * icon) {

    if (NULL != icon && NULL != icon->iconData) {
#if JME_DEBUG
        osalTimeUpdate();
        uint32_t startTime = osal_GetSystemClock();
#endif
        uint16_t fgColor = icon->color;
        uint16_t bgColor = icon->bgColor;
        JMERect imageFrame = JMERectMake(0, 0, icon->iconSize.width, icon->iconSize.height);
        imageFrame.origin.x = icon->iconFrame.origin.x + ((icon->iconFrame.size.width - icon->iconSize.width) >> 1);
        imageFrame.origin.y = icon->iconFrame.origin.y + ((icon->iconFrame.size.height - icon->iconSize.height) >> 1);
        JMERect topInset = JMERectMake(icon->iconFrame.origin.x, icon->iconFrame.origin.y,
                                       icon->iconFrame.size.width, imageFrame.origin.y - icon->iconFrame.origin.y);
        JMERect bottomInset = JMERectMake(icon->iconFrame.origin.x, imageFrame.origin.y + imageFrame.size.height,
                                          icon->iconFrame.size.width, 0);
        bottomInset.size.height = icon->iconFrame.origin.y + icon->iconFrame.size.height - bottomInset.origin.y;
        JMERect leftInset = JMERectMake(icon->iconFrame.origin.x, imageFrame.origin.y,
                                        imageFrame.origin.x - icon->iconFrame.origin.x,
                                        imageFrame.size.height);
        JMERect rightInset = JMERectMake(imageFrame.origin.x + imageFrame.size.width,
                                         imageFrame.origin.y, 0, imageFrame.size.height);
        rightInset.size.width = icon->iconFrame.origin.x + icon->iconFrame.size.width - rightInset.origin.x;

        JMEST7735R_drawRect(topInset, bgColor, TRUE);
        JMEST7735R_drawRect(leftInset, bgColor, TRUE);
        JMEST7735R_drawRect(rightInset, bgColor, TRUE);
        JMEST7735R_drawRect(bottomInset, bgColor, TRUE);
        JMEST7735R_drawBinaryImage(icon->iconData, imageFrame, fgColor, bgColor);
#if JME_DEBUG
        osalTimeUpdate();
        uint32_t endTime = osal_GetSystemClock() - startTime;
        JMEPrintf("cost:%d", endTime);
#endif
    }
}

void JMEST7735R_drawNumber(JMEPoint startPoint, uint16_t number, uint16_t textColor, uint16_t bgColor, uint8_t fontSize)
{
    if (fontSize > 0)
    {
        JMERect numberFrame;
        numberFrame.origin = startPoint;
        numberFrame.size.width = JMEST7735R_NUMBERSIZE.width * fontSize;
        numberFrame.size.height = JMEST7735R_NUMBERSIZE.height * fontSize;
        uint8_t modeNumber = 100;
        if (number > 999) {
            number %= 1000;
        }

        for (uint8_t i = 0; i < 3; i ++) {
            uint8_t displayNumber = number / modeNumber;
            number -= displayNumber * modeNumber;
            modeNumber /= 10;
            if (_JMEST7735R_setDrawWindow(numberFrame))
            {
                _JMEST7735R_write_command(JMEST7735R_RAMWR);
                JMEST7735R_CDSET();
                JMEST7735R_RDSET();
                JMEST7735R_CSCLR();
                for (int row = 0; row < JMEST7735R_NUMBERSIZE.height; row ++)
                {
                    uint8_t bitMask = JMEBit(row);
                    for (int i = 0; i < fontSize; i ++)
                    {
                        for (int col = 0; col < JMEST7735R_NUMBERSIZE.width; col ++)
                        {
                            uint16_t color = bgColor;
                            if (JMEASCII_NUMBER[displayNumber * JMEST7735R_NUMBERSIZE.width + col] & bitMask)
                            {
                                color = textColor;
                            }
                            for (int j = 0; j < fontSize; j ++)
                            {
                                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                                JMEST7735R_seqWrite((uint8_t)color);
                            }
                        }
                    }
                }
                JMEST7735R_CSSET();
            }
            numberFrame.origin.x += numberFrame.size.width;
        }
    }
}

void JMEST7735R_drawMenuIcon(const JMEMenuIcon_t * icon, BOOL isHighLight) {
    if (NULL != icon) {
        if (_JMEST7735R_setDrawWindow(icon->iconFrame))
        {
            _JMEST7735R_write_command(JMEST7735R_RAMWR);
            uint16_t count = icon->iconFrame.size.width * icon->iconFrame.size.height;
            JMERect imageFrame = JMERectMake(0, 0, icon->iconSize.width, icon->iconSize.height);
            imageFrame.origin.x = (icon->iconFrame.size.width - icon->iconSize.width) >> 1;
            imageFrame.origin.y = (icon->iconFrame.size.height - icon->iconSize.height) >> 1;
#if JME_DEBUG
            osalTimeUpdate();
            uint32_t startTime = osal_GetSystemClock();
#endif
            JMEST7735R_CDSET();
            JMEST7735R_RDSET();
            JMEST7735R_CSCLR();
            uint16_t color = icon->color;
            count = imageFrame.origin.y * icon->iconFrame.size.width;
            while (count --) {
                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                JMEST7735R_seqWrite((uint8_t)color);
            }
            for (uint8_t r = 0; r < imageFrame.size.height; r ++) {
                uint16_t offset = r * imageFrame.size.width;
                for (uint8_t c = 0; c < icon->iconFrame.size.width; c ++) {
                    if (c >= imageFrame.origin.x && c < imageFrame.origin.x + imageFrame.size.width) {
                        color = ((uint16_t *)(icon->iconData))[offset + c - imageFrame.origin.x];
                    } else {
                        color = icon->color;
                    }
                    JMEST7735R_seqWrite((uint8_t)(color >> 8));
                    JMEST7735R_seqWrite((uint8_t)color);
                }
            }
            color = icon->color;
            count = (icon->iconFrame.size.height - imageFrame.size.height - imageFrame.origin.y) * icon->iconFrame.size.width;
            while (count --) {
                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                JMEST7735R_seqWrite((uint8_t)color);
            }
            JMEST7735R_CSSET();
#if JME_DEBUG
            osalTimeUpdate();
            uint32_t endTime = osal_GetSystemClock() - startTime;
            JMEPrintf("cost:%d", endTime);
#endif
        }
    }
}

void JMEST7735R_drawString(JMEPoint startPoint, const char * string,
                           uint16_t textColor, uint16_t bgColor, uint8_t fontSize) {
    if (fontSize > 0)
    {
        JMERect charFrame;
        charFrame.origin = startPoint;
        charFrame.size.width = JMEST7735R_ASCIISIZE.width * fontSize;
        charFrame.size.height = JMEST7735R_ASCIISIZE.height * fontSize;
        for (uint8_t i = 0; i < strlen(string); i ++) {
            char character = string[i] - 32;
            if (_JMEST7735R_setDrawWindow(charFrame))
            {
                _JMEST7735R_write_command(JMEST7735R_RAMWR);
                JMEST7735R_CDSET();
                JMEST7735R_RDSET();
                JMEST7735R_CSCLR();
                for (int row = 0; row < JMEST7735R_ASCIISIZE.height; row ++)
                {
                    for (int i = 0; i < fontSize; i ++)
                    {
                        for (int col = 0; col < JMEST7735R_ASCIISIZE.width; col ++)
                        {
                            uint16_t color = bgColor;
                            if (kJME_ASCII8x12_Table[character * JMEST7735R_ASCIISIZE.height + row] & JMEBit(7-col))
                            {
                                color = textColor;
                            }
                            for (int j = 0; j < fontSize; j ++)
                            {
                                JMEST7735R_seqWrite((uint8_t)(color >> 8));
                                JMEST7735R_seqWrite((uint8_t)color);
                            }
                        }
                    }
                }
                JMEST7735R_CSSET();
            }
            charFrame.origin.x += charFrame.size.width;
        }
    }
}

/*********************************************************************
 * IMPLEMENT OF PRIVATE FUNCTIONS
 */
#pragma mark - private functions
static inline void _JMEST7735R_HDReset(void)
{
    JMEST7735R_RESETENABLE();
    JMEST7735R_delayMS(100);
    JMEST7735R_RESETDISABLE();
    JMEST7735R_delayMS(100);
}

static inline void _JMEST7735R_SWReset(void)
{
    _JMEST7735R_write_command(JMEST7735R_SWReset);
    JMEST7735R_delayMS(120);
}

static inline void _JMEST7735R_setPixelFormat(JMEST7735R_IPF pixelFormat)
{
    _JMEST7735R_write_command(JMEST7735R_COLMOD);
    _JMEST7735R_write_data(pixelFormat);
}

static inline BOOL _JMEST7735R_setDrawWindow(JMERect rect) {
    if (!JMERectIsEmpty(rect)) {
        uint8_t x0 = rect.origin.x + kJMEST7735RScreenFrame.origin.x;
        uint8_t x1 = x0 + rect.size.width - 1;
        uint8_t y0 = rect.origin.y + kJMEST7735RScreenFrame.origin.y;
        uint8_t y1 = y0 + rect.size.height - 1;
        //
        // column address set
        _JMEST7735R_write_command(JMEST7735R_CASET);
        _JMEST7735R_write_data(0x00); _JMEST7735R_write_data(x0);
        _JMEST7735R_write_data(0x00); _JMEST7735R_write_data(x1);
        //
        // row address set
        _JMEST7735R_write_command(JMEST7735R_RASET);
        _JMEST7735R_write_data(0x00); _JMEST7735R_write_data(y0);
        _JMEST7735R_write_data(0x00); _JMEST7735R_write_data(y1);
        return TRUE;
    }
    return FALSE;
}

static inline void _JMEST7735R_writePixelData(const uint16_t * colorArray, uint16_t count)
{
    _JMEST7735R_write_command(JMEST7735R_RAMWR);

    JMEST7735R_CDSET();
    JMEST7735R_RDSET();
    JMEST7735R_CSCLR();
    for (int i = 0; i < count; i ++) {
        uint16_t color = colorArray[i];
        JMEST7735R_seqWrite((uint8_t)(color >> 8));
        JMEST7735R_seqWrite((uint8_t)color);
    }
    JMEST7735R_CSSET();
}

static inline void _JJMEST7735R_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
    if (_JMEST7735R_setDrawWindow(JMERectMake(x, y, 1, 1))) {
        _JMEST7735R_writePixelData(&color, 1);
    }
}

static inline void _JMEST7735R_write_command(uint8_t cmd) {
    JMEST7735R_CSCLR();
    JMEST7735R_CDCLR();

    JMEST7735R_RDSET();
    JMEST7735R_RWCLR();
    JMEST7735R_writeByte(cmd);
    JMEST7735R_RWSET();
    JMEST7735R_NOP();JMEST7735R_NOP();

    JMEST7735R_CSSET();
}

static inline void _JMEST7735R_write_data(uint8_t data) {
    JMEST7735R_CSCLR();
    JMEST7735R_CDSET();

    JMEST7735R_RDSET();
    JMEST7735R_RWCLR();
    JMEST7735R_writeByte(data);
    JMEST7735R_RWSET();
    JMEST7735R_NOP();JMEST7735R_NOP();

    JMEST7735R_CSSET();
}

static inline uint8_t _JMEST7735R_read_data(void) {
    uint8_t data = 0;

    JMEST7735R_CSCLR();
    JMEST7735R_CDSET();
    JMEST7735R_RWSET();
    JMEST7735R_RDCLR();
    JMEST7735R_NOP();JMEST7735R_NOP();
    JMEST7735R_RDSET();
    data = JMEST7735R_readByte();

    return data;
}
