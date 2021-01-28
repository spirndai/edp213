/**
 *  @filename   :   epd2in13.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epd2in13.h"
#include "common.h"
void sendbyte(int sdbyte)
{
    epdcslow;
    volatile unsigned int i;
    for (i = 0; i < 8; i++)
    {
        epdcklow;
        if (sdbyte & 0x80)
        {
            epddahigh;
        }
        else
        {
            epddalow;
        }
        sdbyte <<= 1;
        epdckhigh;
    }
    epdcklow;
    epdcshigh;
}
void InitGpio(void)
{
  rfcslow;
  rfcklow;
  rfdalow;
  spiromcslow;
  epdunknowhigh;
  epdbslow ;
  epdon;
  rfoff;
  P1DIR = BIT2 | BIT3 | BIT4 | BIT6 | BIT7 ;
  P2DIR = BIT3 | BIT4 | BIT5 | BIT6 | BIT7 ;
  P3DIR = BIT0 | BIT1 | BIT4 | BIT5 | BIT6 | BIT7 | BIT3;
  P1SEL = 0X0;
  P1SEL2 = 0X0;
  P2SEL = 0X0;
  P2SEL2 = 0X0;
  P3SEL = 0X0;
  P3SEL2 = 0X0;
  Delay(200);
  epdrsthigh;
  Delay(200);
  epdrstlow;
  Delay(200);
  epdrsthigh;

}

void InitClk(void)
{
  DCOCTL = 0x0000;
  BCSCTL1 = CALBC1_8MHZ;                    // Set range
  DCOCTL = CALDCO_8MHZ;

  BCSCTL3 |= LFXT1S_2;                   /* Mode 2 for LFXT1 : VLO */
  IFG1 &= !(OFIFG);
  Delay(512);
  BCSCTL1 |= DIVA_3 ;                     /* ACLK Divider 3: /8 */
}

const unsigned char *this_lut;
int this_width = 128;
int this_height = 250 ;
int Edp_Init(const unsigned char *lut)
{
    InitGpio();
    InitClk();


    /* EPD hardware init start */
    this_lut = lut;
    Edp_Reset();
    Edp_SendCommand(DRIVER_OUTPUT_CONTROL);
    Edp_SendData((this_height - 1) & 0xFF);
    Edp_SendData(((this_height - 1) >> 8) & 0xFF);
    Edp_SendData(0x00);                     // GD = 0; SM = 0; TB = 0;
    Edp_SendCommand(BOOSTER_SOFT_START_CONTROL);
    Edp_SendData(0xD7);
    Edp_SendData(0xD6);
    Edp_SendData(0x9D);
    Edp_SendCommand(WRITE_VCOM_REGISTER);
    Edp_SendData(0xA8);                     // VCOM 7C
    Edp_SendCommand(SET_DUMMY_LINE_PERIOD);
    Edp_SendData(0x1A);                     // 4 dummy lines per gate
    Edp_SendCommand(SET_GATE_TIME);
    Edp_SendData(0x08);                     // 2us per line
    Edp_SendCommand(DATA_ENTRY_MODE_SETTING);
    Edp_SendData(0x03);                     // X increment; Y increment
    Edp_SetLut(this_lut);
    /* EPD hardware init end */
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Edp_SendCommand(unsigned char command)
{
    //DigitalWrite(dc_pin, LOW);
    epddclow;
    sendbyte(command);

}

/**
 *  @brief: basic function for sending data
 */
void Edp_SendData(unsigned char data)
{
    //DigitalWrite(dc_pin, HIGH);
    epddchigh;
    sendbyte(data);
}

/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void Edp_WaitUntilIdle(void)
{
    while (epdbsread > 0)
    {      //LOW: idle, HIGH: busy
        DelayMs(100);
    }
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Edp_Sleep();
 */
void Edp_Reset(void)
{
    //DigitalWrite(reset_pin, LOW);                //module reset
    epdrstlow;
    DelayMs(200);
    //DigitalWrite(reset_pin, HIGH);
    epdrsthigh;
    DelayMs(200);
}

/**
 *  @brief: set the look-up table register
 */
void Edp_SetLut(const unsigned char *lut)
{
    int i ;
    this_lut = lut;
    Edp_SendCommand(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (i = 0; i < 30; i++)
    {
        Edp_SendData(this_lut[i]);
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void Edp_SetFrameMemory_Part(const unsigned char *image_buffer, int x, int y, int image_width, int image_height)
{
    int x_end;
    int y_end;
    int i,j;

    if (image_buffer == NULL || x < 0 || image_width < 0 || y < 0 || image_height < 0)
    {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= this_width)
    {
        x_end = this_width - 1;
    }
    else
    {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this_height)
    {
        y_end = this_height - 1;
    }
    else
    {
        y_end = y + image_height - 1;
    }
    Edp_SetMemoryArea(x, y, x_end, y_end);
    /* set the frame memory line by line */
    for (j = y; j <= y_end; j++)
    {
        Edp_SetMemoryPointer(x, j);
        Edp_SendCommand(WRITE_RAM);
        for ( i = x / 8; i <= x_end / 8; i++)
        {
            Edp_SendData(image_buffer[(i - x / 8) + (j - y) * (image_width / 8)]);
        }
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 *
 *          Question: When do you use this function instead of 
 *          void SetFrameMemory(
 *              const unsigned char* image_buffer,
 *              int x,
 *              int y,
 *              int image_width,
 *              int image_height
 *          );
 *          Answer: SetFrameMemory with parameters only reads image data
 *          from the RAM but not from the flash in AVR chips (for AVR chips,
 *          you have to use the function pgm_read_byte to read buffers 
 *          from the flash).
 */
void Edp_SetFrameMemory(const unsigned char *image_buffer)
{
    int i,j;
    Edp_SetMemoryArea(0, 0, this_width - 1, this_height - 1);
    /* set the frame memory line by line */
    for (j = 0; j < this_height; j++)
    {
        Edp_SetMemoryPointer(0, j);
        Edp_SendCommand(WRITE_RAM);
        for (i = 0; i < this_width / 8; i++)
        {
            Edp_SendData(image_buffer[i + j * (this_width / 8)]);
        }
    }
}

/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
void Edp_ClearFrameMemory(unsigned char color)
{
    int i,j;
    Edp_SetMemoryArea(0, 0, this_width - 1, this_height - 1);
    /* set the frame memory line by line */
    for (j = 0; j < this_height; j++)
    {
        Edp_SetMemoryPointer(0, j);
        Edp_SendCommand(WRITE_RAM);
        for (i = 0; i < this_width / 8; i++)
        {
            Edp_SendData(color);
        }
    }
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will 
 *          set the other memory area.
 */
void Edp_DisplayFrame(void){
    Edp_SendCommand(DISPLAY_UPDATE_CONTROL_2);
    Edp_SendData(0xC4);
    Edp_SendCommand(MASTER_ACTIVATION);
    Edp_SendCommand(TERMINATE_FRAME_READ_WRITE);
    Edp_WaitUntilIdle();
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
void Edp_SetMemoryArea(int x_start, int y_start, int x_end, int y_end)
{
    Edp_SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    Edp_SendData((x_start >> 3) & 0xFF);
    Edp_SendData((x_end >> 3) & 0xFF);
    Edp_SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    Edp_SendData(y_start & 0xFF);
    Edp_SendData((y_start >> 8) & 0xFF);
    Edp_SendData(y_end & 0xFF);
    Edp_SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void Edp_SetMemoryPointer(int x, int y)
{
    Edp_SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    Edp_SendData((x >> 3) & 0xFF);
    Edp_SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    Edp_SendData(y & 0xFF);
    Edp_SendData((y >> 8) & 0xFF);
    Edp_WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          You can use Edp_Init() to awaken
 */
void Edp_Sleep()
{
    Edp_SendCommand(DEEP_SLEEP_MODE);
    Edp_WaitUntilIdle();
}

const unsigned char lut_full_update[] = { 0x22, 0x55, 0xAA, 0x55, 0xAA, 0x55,
                                          0xAA, 0x11, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x1E, 0x1E,
                                          0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
                                          0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };

const unsigned char lut_partial_update[] =
        { 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* END OF FILE */

