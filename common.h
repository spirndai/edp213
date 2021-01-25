#ifndef COMMON_H
#define  COMMON_H
#define epdcklow           P2OUT &= 0xf7 //P2_3
#define epdckhigh          P2OUT |= 0x08
#define epddalow           P2OUT &= 0xef //P2_4
#define epddahigh          P2OUT |= 0x10
#define epdcslow           P3OUT &= 0xef //P3_4
#define epdcshigh          P3OUT |= 0x10
#define epddclow           P3OUT &= 0xdf// P3_5
#define epddchigh          P3OUT |= 0x20
#define epdrstlow          P3OUT &= 0xbf // P3_6
#define epdrsthigh         P3OUT |= 0x40
#define epdbslow          P3OUT &= 0xfd //P3_1
#define epdunknowhigh          P3OUT &= 0x80  //P3_7
#define epdon           P2OUT &= 0xbf //p2_6
#define epdoff          P2OUT |= 0x40

#define epdbsread          (P3IN & 0x02) //P3_1

#define rfcklow            P1OUT &= 0xef
#define rfckhigh           P1OUT |= 0x10
#define rfdalow            P1OUT &= 0xfb
#define rfdahigh           P1OUT |= 0x04
#define rfcslow            P1OUT &= 0xf7
#define rfcshigh           P1OUT |= 0x08
#define rfon               P2OUT &= 0x7f
#define rfoff              P2OUT |= 0x80

#define spiromcshigh       P3OUT |= 0x01
#define spiromcslow        P3OUT &= 0xfe

#define p20high       P2OUT |= 0x01
#define p20low        P2OUT &= 0xfe
#define p21high          P2OUT |= 0x02
#define p21low          P2OUT &= 0xfd
#define p22high          P2OUT |= 0x04
#define p22low          P2OUT &= 0xfb
#define p32high          P3OUT |= 0x04
#define p32low          P3OUT &= 0xfb
#define p33low           P3OUT &= 0xf7
#define p33high          P3OUT |= 0x08

#define DelayMs Delay
void Delay(unsigned int nCount);

void Delaylong(unsigned int n10Count);
#endif
