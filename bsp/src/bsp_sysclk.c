#include "bsp.h"


/******************************************************************************
**   ʱ�ӳ�ʼ������
******************************************************************************/
void init_CLK(void)
{
  P5SEL |= BIT2+BIT3;                     
  UCSCTL6 &= ~XT2OFF;   //ʹ���ⲿ����                  
  
  __bis_SR_register(SCG0);                
  
  UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;    
  UCSCTL1 = DCORSEL_3;                   
  
  UCSCTL2 = FLLD_2+7;                    
  UCSCTL3 = SELREF_5 + FLLREFDIV_5;       
  UCSCTL4 = SELA_2 + SELS_5 +SELM_3;                                         
  __bic_SR_register(SCG0);                
  do
  {
          UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); 
          SFRIFG1 &= ~OFIFG;                          
  }while (SFRIFG1&OFIFG);
}