/**
 *  @filename   :   epd2in13-demo.ino
 *  @brief      :   2.13inch e-paper display demo
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

#include "epd2in13.h"
#include "epdPaint.h"
#include "common.h"
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

unsigned long time_now_s;

int main(void)
{
    // put your setup code here, to run once:

    Paint_init(1024);
    if (Edp_Init(lut_full_update) != 0)
    {
        return 1;
    }

    Edp_ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    Paint_SetRotate(ROTATE_0);
    Paint_SetWidth(128);    // width should be the multiple of 8
    Paint_SetHeight(24);
    Paint_Clear(COLORED);
    Paint_DrawStringAt(30, 4, "Hello world!", &Font8, UNCOLORED);
    Edp_SetFrameMemory_Part(Paint_GetImage(), 0, 10, Paint_GetWidth(),
                            Paint_GetHeight());

    Paint_Clear(UNCOLORED);
    Paint_DrawStringAt(30, 4, "e-Paper Demo", &Font8, COLORED);
    Edp_SetFrameMemory_Part(Paint_GetImage(), 0, 30, Paint_GetWidth(),
                            Paint_GetHeight());

    Paint_SetWidth(64);
    Paint_SetHeight(64);

    Paint_Clear(UNCOLORED);
    Paint_DrawRectangle(0, 0, 40, 50, COLORED);
    Paint_DrawLine(0, 0, 40, 50, COLORED);
    Paint_DrawLine(40, 0, 0, 50, COLORED);
    Edp_SetFrameMemory_Part(Paint_GetImage(), 16, 60, Paint_GetWidth(),
                            Paint_GetHeight());

    Paint_Clear(UNCOLORED);
    Paint_DrawCircle(32, 32, 30, COLORED);
    Edp_SetFrameMemory_Part(Paint_GetImage(), 72, 60, Paint_GetWidth(),
                            Paint_GetHeight());

    Paint_Clear(UNCOLORED);
    Paint_DrawFilledRectangle(0, 0, 40, 50, COLORED);
    Edp_SetFrameMemory_Part(Paint_GetImage(), 16, 130, Paint_GetWidth(),
                            Paint_GetHeight());

    Paint_Clear(UNCOLORED);
    Paint_DrawFilledCircle(32, 32, 30, COLORED);
    Edp_SetFrameMemory_Part(Paint_GetImage(), 72, 130, Paint_GetWidth(),
                            Paint_GetHeight());
    Edp_DisplayFrame();

    Delay(2000);

    if (Edp_Init(lut_partial_update) != 0)
    {
        // Serial.print("e-Paper init failed");
        return 1;
    }

    /**
     *  there are 2 memory areas embedded in the e-paper display
     *  and once the display is refreshed, the memory area will be auto-toggled,
     *  i.e. the next action of SetFrameMemory will set the other memory area
     *  therefore you have to set the frame memory and refresh the display twice.
     */

    Edp_SetFrameMemory(IMAGE_DATA);
    Edp_DisplayFrame();
    Edp_SetFrameMemory(IMAGE_DATA);
    Edp_DisplayFrame();

    while (1)
    {
        // put your main code here, to run repeatedly:
        time_now_s = 0;
        char time_string[] = { '0', '0', ':', '0', '0', '\0' };
        time_string[0] = time_now_s / 60 / 10 + '0';
        time_string[1] = time_now_s / 60 % 10 + '0';
        time_string[3] = time_now_s % 60 / 10 + '0';
        time_string[4] = time_now_s % 60 % 10 + '0';
        Paint_SetWidth(32);
        Paint_SetHeight(96);
        Paint_SetRotate(ROTATE_90);
        Paint_Clear(UNCOLORED);
        Paint_DrawStringAt(0, 4, time_string, &Font8, COLORED);
        Edp_SetFrameMemory_Part(Paint_GetImage(), 80, 72, Paint_GetWidth(),
                                Paint_GetHeight());
        Edp_DisplayFrame();

        Delay(500);
    }
}
