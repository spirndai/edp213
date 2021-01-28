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
unsigned char imgbuf[384];
unsigned long time_now_s = 0;
#define TIMER_PERIOD (10680)
void showback()
{
    Edp_Init(lut_full_update);
    Edp_SetFrameMemory(IMAGE_DATA);
    Edp_DisplayFrame();
    Delaylong(5);
}
int main(void)
{
    int isback = 0;
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    Paint_init(imgbuf);

    // reset timer A config (not strictly needed)
    TACTL = TACLR;

    // ACLK as clock source, divider 8, continuous mode, interrupt enabled
    TACTL = TASSEL_1 | ID_3 | MC_2 | TAIE;

    // set the period
    TACCR1 = TIMER_PERIOD;

    // enable capture/compare interrupts for CCR1
    TACCTL1 = CCIE;
    _enable_interrupts();

    while (1)
    {

        if (time_now_s % 5 == 0 && isback == 0)
        {
            showback();
            Edp_Init(lut_partial_update);
            isback = 1;
        }
        if (time_now_s % 5 != 0)
            isback = 0;

        char time_string[] = { '0', '0', ':', '0', '0', '\0' };
        time_string[0] = time_now_s / 60 / 10 + '0';
        time_string[1] = time_now_s / 60 % 10 + '0';
        time_string[3] = time_now_s % 60 / 10 + '0';
        time_string[4] = time_now_s % 60 % 10 + '0';
        Paint_SetWidth(32);
        Paint_SetHeight(96);
        Paint_SetRotate(ROTATE_90);
        Paint_Clear(UNCOLORED);
        Paint_DrawStringAt(0, 4, time_string, &Font24, COLORED);
        Edp_SetFrameMemory_Part(Paint_GetImage(), 80, 72, Paint_GetWidth(),
                                Paint_GetHeight());
        Edp_DisplayFrame();
        Delay(1000);
    }

}
#pragma vector=TIMER0_A1_VECTOR

__interrupt void Timer_A(void)
{
    switch (TAIV)
    {
    case 0x02:
        time_now_s++;
        // set the time of the next interrupt
        TACCR1 += TIMER_PERIOD;
        break;
    }

}
