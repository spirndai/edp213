#include "common.h"
void Delay(unsigned int nCount)
{
  for(;nCount!=0;nCount--)
  {
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
  }

}

void Delaylong(unsigned int n10Count)
{
        unsigned char i;
            while(n10Count--)
            {
              for(i = 0; i < 10; i ++)
                {
                        Delay(10000);
                }
            }
}
