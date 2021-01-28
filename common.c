#include "common.h"
void Delay(unsigned int nCount)
{
    int i;
    for (; nCount != 0; nCount--)
    {
        for (i = 0; i < 940; i++)
        {
            asm(" nop");
            asm(" nop");
            asm(" nop");
            asm(" nop");
        }
    }

}

void Delaylong(unsigned int n10Count)
{
    while (n10Count--)
    {

        Delay(1000);
    }
}
