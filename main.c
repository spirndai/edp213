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
#include "common.h"

#define COLORED     0
#define UNCOLORED   1





unsigned long time_now_s;



int main(void) {
  // put your setup code here, to run once:

  if (Edp_Init(lut_full_update) != 0) {

      return 1;
  }

  Edp_ClearFrameMemory(0xFF);   // bit set = white, bit reset = black

  //Edp_SetFrameMemory_Part(paint.GetImage(), 72, 130, paint.GetWidth(), paint.GetHeight());
  Edp_DisplayFrame();

  Delay(2000);

  if (Edp_Init(lut_partial_update) != 0) {
     // Serial.print("e-Paper init failed");
      return 1;
  }

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */

//  Edp_SetFrameMemory(IMAGE_DATA);
  Edp_DisplayFrame();
//  Edp_SetFrameMemory(IMAGE_DATA);
  Edp_DisplayFrame();



while (1){
  // put your main code here, to run repeatedly:
  time_now_s =0 ;
  char time_string[] = {'0', '0', ':', '0', '0', '\0'};
  time_string[0] = time_now_s / 60 / 10 + '0';
  time_string[1] = time_now_s / 60 % 10 + '0';
  time_string[3] = time_now_s % 60 / 10 + '0';
  time_string[4] = time_now_s % 60 % 10 + '0';


//  Edp_SetFrameMemory_Part(paint.GetImage(), 80, 72, paint.GetWidth(), paint.GetHeight());
  Edp_DisplayFrame();

  Delay(500);
}
}
