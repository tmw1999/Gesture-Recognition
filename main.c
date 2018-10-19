#include "bsp.h"
#include "math.h"
#include "string.h"
#define uchar unsigned char
unsigned int kong,jiandao,shitou,bu,one,two,three,four,five;
uchar spkshitou=0,spkjiandao=0,spkbu=0,spkone=0,spktwo=0,spkthree=0,spkfour=0,spkfive=0;
uchar P11num = 0;
uchar P11num1 = 0;
uchar taba[]=   {"正在猜拳训练    "};
uchar tabb[]=   {"猜拳判决        "};
uchar tabc[]=   {"正在划拳训练    "};
uchar tabd[]=   {"划拳判决        "};
uchar tabkong[]={"    空          "};
uchar tab2[]=   {"    剪刀        "};
uchar tab3[]=   {"    石头        "};
uchar tab4[]=   {"    布          "};
uchar tabshitou[]={"学习石头手势    "};
uchar tabjiandao[]={"学习剪刀手势    "};
uchar tabbu[]={"学习布的手势    "};
uchar tabend[]={"猜拳训练完成    "};
uchar tabend1[]={"划拳训练完成    "};
uchar tabinit[]={"正在记录初值    "};
uchar tabone[]={"    一          "};
uchar tabtwo[]={"    二          "};
uchar tabthree[]={"    三          "};
uchar tabfour[]={"    四          "};
uchar tabfive[]={"    五          "};
uchar tablearnone[]={"学习一的手势    "};
uchar tablearntwo[]={"学习二的手势    "};
uchar tablearnthree[]={"学习三的手势    "};
uchar tablearnfour[]={"学习四的手势    "};
uchar tablearnfive[]={"学习五的手势    "};
uchar tabledata[]={"                "};
uchar welcome0[]={"欢迎使用"};
uchar welcome1[]={"手势识别装置"};
uchar mode = 0;
unsigned long  temp,temp1,temp2,tempand,tem[6]={0,0,0,0,0,0};
unsigned int *pFlash;
unsigned char kongstate=0;   

void delay5ms(void)
{
  unsigned int i = 4000;   //40000
  while(i!=0)
  {
    i--;
  }
}

void write_cmd(uchar cmd)
{
  P1OUT&= ~BIT4;
  P1OUT&= ~BIT6;
  P1OUT|=  BIT5;
  delay5ms();
  P3OUT = cmd;
  delay5ms();
  P1OUT&= ~BIT5;
}

void write_dat(uchar dat)
{
  P1OUT|= BIT4;
  P1OUT&= ~BIT6;
  P1OUT|=  BIT5;
  delay5ms();
  P3OUT = dat;
  delay5ms();
  P1OUT&= ~BIT5;
}

void  lcd_pos(uchar x,uchar y)
{
  uchar pos;
  if(x == 0)
  {x = 0x80;}
  else if (x == 1)
  {x = 0x90;}
  else if (x == 2)
  {x = 0x88;}
  else if (x == 3)
  {x = 0x98;}
  pos = x + y;
  write_cmd(pos);
}

void LCD_init(void)
{
  write_cmd(0x38);
  delay5ms();
  write_cmd(0x0C);
  delay5ms();
  write_cmd(0x01);
  delay5ms();
}

void init_port(void)
{
  P1DIR &= ~BIT1;
  P2DIR &= ~BIT1;
  P1IES |= BIT1;
  P2IES |= BIT1;
  P1OUT |= BIT1;
  P2OUT |= BIT1;
  P1REN |= BIT1;
  P2REN |= BIT1;
  P1IE  |= BIT1;
  P2IE  |= BIT1;
  P1IFG &= ~BIT1;
  P2IFG &= ~BIT1;
  P2IFG = 0;
  P1IFG = 0;
}
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
 // if(P2IN&BIT1  == 0)
  delay_ms(500); //200
  { mode++;
  if(mode >= 5)
    mode =1;
  if(mode == 1)         //正在猜拳训练
  {
   
    lcd_pos(0,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(taba[i]);     
        }
    lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabinit[i]);     
        }
                      putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x0B);
                      putchar(0xBE);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC);   
       
       
       
       
  }
    if(mode == 2)       //猜拳判决       
  {
    lcd_pos(0,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabb[i]);     
        }
     lcd_pos(1,0);
     for(int i = 0;i<16;i++)
        {
          write_dat(tabledata[i]);    //空格冲掉 
        }
                      putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x0C);
                      putchar(0xBF);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC);     
     pFlash = (unsigned int *)0x1800;
     kong = *pFlash++;
     shitou = *pFlash++;
     jiandao = *pFlash++;
     bu = *pFlash;
  }
  if(mode == 3)         //划拳学习
  {
    lcd_pos(0,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabc[i]);     
        }
    lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabinit[i]);     
        }   
       
                      putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x0D);
                      putchar(0xC0);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC); 
  }
  if(mode == 4)         //划拳判决
  {
    lcd_pos(0,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabd[i]);     
        }
    lcd_pos(1,0);
     for(int i = 0;i<16;i++)
        {
          write_dat(tabledata[i]);     //空格冲掉
        }
                       putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x09);
                      putchar(0xBC);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC);     
     pFlash = (unsigned int *)0x1900;//0x4000;
     kong = *pFlash++;
     one = *pFlash++;
     two = *pFlash++;
     three = *pFlash++;
     four = *pFlash++;
     five = *pFlash;
  }
  }
  P2IFG = 0;
}
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
 // if(P1IN&BIT1  == 0)
    delay_ms(500); //200
    //猜拳的学习
    if(mode == 1&&P11num == 0)
    {  kong = (unsigned int)temp;
       lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabshitou[i]);     
        }
    }
    if(mode == 1&&P11num == 1)
    {
      shitou = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabjiandao[i]);     
        }
    }
    if(mode == 1&&P11num == 2)
    {
      jiandao = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabbu[i]);     
        }
    }
    if(mode == 1&&P11num == 3)
    {
      bu = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabend[i]);     
        }
        pFlash = (unsigned int *)0x1800;
        FCTL3 = FWKEY;
        FCTL1 = FWKEY + ERASE;
        *pFlash = 0;
        while(FCTL3 & BUSY);
        pFlash = (unsigned int *)0x1800;
        FCTL1 = FWKEY + WRT;
        *pFlash++ = kong;
        *pFlash++ = shitou;
        *pFlash++ = jiandao;
        *pFlash = bu;
        FCTL1 = FWKEY;
        FCTL3 = FWKEY + LOCK;
    }
    if(mode == 1)
    {
      P11num++;
        if(P11num>=4)
          P11num = 0;
    }
        //划拳手势的学习
    if(mode == 3&&P11num1 == 0)
    {
      kong = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tablearnone[i]);     
        }
    }
    if(mode == 3&&P11num1 == 1)
    {
      one = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tablearntwo[i]);     
        }
    }
    if(mode == 3&&P11num1 == 2)
    {
      two = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tablearnthree[i]);     
        }
    }
    if(mode == 3&&P11num1 == 3)
    {
      three = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tablearnfour[i]);     
        }
    }
    if(mode == 3&&P11num1 == 4)
    {
      four = (unsigned int)temp;
      lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tablearnfive[i]);     
        }
    }
    if(mode == 3&&P11num1 == 5)
    {
        five = (unsigned int)temp;
        lcd_pos(1,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabend1[i]);     
        }
        pFlash = (unsigned int *)0x1900;//0x4000;
        FCTL3 = FWKEY;
        FCTL1 = FWKEY + ERASE;
        *pFlash = 0;
        while(FCTL3 & BUSY);
        FCTL1 = FWKEY + WRT;
        pFlash = (unsigned int *)0x1900;//0x4000;
        *pFlash++ = kong;
        *pFlash++ = one;
        *pFlash++ = two;
        *pFlash++ = three;
        *pFlash++ = four;
        *pFlash = five;
        FCTL1 = FWKEY;
        FCTL3 = FWKEY + LOCK;
    }   
     if(mode == 3)
     {
       P11num1++;
        if(P11num1>=6)
          P11num1 = 0;   
     }
        P1IFG = 0;   
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
 
  TA0IV=0;
  if(mode == 2)
  {
      if(temp<kong+(kong-shitou)/2&&temp>kong-(kong-shitou)/2+5)
      {
        TA0R=58982;
        kongstate = 0;
        lcd_pos(2,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabkong[i]);     
        }
      }
      else
      {   
            TA0R=50000;
            //printf("%s\n","空");
            if(kongstate ==1)
             {
               if(mode == 2)
               {
                  if(temp<shitou+(kong-shitou)/2-5&&temp>shitou-(shitou-jiandao)/2+5)//printf("%s\n","石头");
                  {
                    lcd_pos(2,0);
                    for(int i = 0;i<16;i++)
                    {
                      write_dat(tab3[i]);       
                    }
                    if(spkshitou%2==0)
                    {
                      putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x01);
                      putchar(0xB4);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC);
                    }
                    spkshitou++;
                     
                  }
               
                  if(temp<jiandao+(shitou-jiandao)/2-5&&temp>jiandao-(jiandao-bu)/2+5)//printf("%s\n","剪刀");
                    {
                      lcd_pos(2,0);
                      for(int i = 0;i<16;i++)
                      {
                        write_dat(tab2[i]);       
                      }
                      if(spkjiandao%2==0)
                      {
                          putchar(0xAA);
                          putchar(0x07);
                          putchar(0x02);
                          putchar(0x00);
                          putchar(0x02);
                          putchar(0xB5);
                         
                          putchar(0xAA);
                          putchar(0x02);
                          putchar(0x00);
                          putchar(0xAC);
                      }
                      spkjiandao++;
                    }     
             

                  if(temp<bu+(jiandao-bu)/2-5&&temp>bu-(jiandao-bu)/2)  //布
                    {
                      lcd_pos(2,0);
                      for(int i = 0;i<16;i++)
                      {
                        write_dat(tab4[i]);
                      }
                      if(spkbu%2==0)
                      {
                          putchar(0xAA);
                          putchar(0x07);
                          putchar(0x02);
                          putchar(0x00);
                          putchar(0x03);
                          putchar(0xB6);
                         
                          putchar(0xAA);
                          putchar(0x02);
                          putchar(0x00);
                          putchar(0xAC);
                      }
                      spkbu++;
                    }
                  }               
               }           
               kongstate =1;
      } 
    }
           
   if(mode == 4)
  {
      if(temp<kong+(kong-one)/2&&temp>kong-(kong-one)/2+5)
      {
        TA0R=58982;
        kongstate = 0;
        lcd_pos(2,0);
        for(int i = 0;i<16;i++)
        {
          write_dat(tabkong[i]);     
        }
      }
      else
      {   
            TA0R=50000;
 
       if(kongstate ==1)
         {
           if(mode == 4)
            {
              if(temp<one+(kong-one)/2-5&&temp>one-(one-two)/2+10)
              {
                    lcd_pos(2,0);
                    for(int i = 0;i<16;i++)
                    {
                      write_dat(tabone[i]);
                    }
                    if(spkone%2==0)
                    {
                            putchar(0xAA);
                            putchar(0x07);
                            putchar(0x02);
                            putchar(0x00);
                            putchar(0x04);
                            putchar(0xB7);
                           
                            putchar(0xAA);
                            putchar(0x02);
                            putchar(0x00);
                            putchar(0xAC);
                    }
                    spkone++;
              }
              if(temp<two+(one-two)/2-10&&temp>two-(two-three)/2+10)
              {
                    lcd_pos(2,0);
                    for(int i = 0;i<16;i++)
                    {
                      write_dat(tabtwo[i]);
                    }
                    if(spktwo%2==0)
                    {
                            putchar(0xAA);
                            putchar(0x07);
                            putchar(0x02);
                            putchar(0x00);
                            putchar(0x05);
                            putchar(0xB8);
                           
                            putchar(0xAA);
                            putchar(0x02);
                            putchar(0x00);
                            putchar(0xAC);
                    }
                    spktwo++;
              }
              if(temp<three+(two-three)/2-10&&temp>three-(three-four)/2+10)
              {
                    lcd_pos(2,0);
                    for(int i = 0;i<16;i++)
                    {
                      write_dat(tabthree[i]);
                    }
                    if(spkthree%2==0)
                    {
                          putchar(0xAA);
                          putchar(0x07);
                          putchar(0x02);
                          putchar(0x00);
                          putchar(0x06);
                          putchar(0xB9);
                         
                          putchar(0xAA);
                          putchar(0x02);
                          putchar(0x00);
                          putchar(0xAC);
                    }
                    spkthree++;
              }
              if(temp<four+(three-four)/2-10&&temp>four-(four-five)/2+10)
              {
                    lcd_pos(2,0);
                    for(int i = 0;i<16;i++)
                    {
                      write_dat(tabfour[i]);
                    }
                    if(spkfour%2==0)
                    {
                            putchar(0xAA);
                            putchar(0x07);
                            putchar(0x02);
                            putchar(0x00);
                            putchar(0x07);
                            putchar(0xBA);
                           
                            putchar(0xAA);
                            putchar(0x02);
                            putchar(0x00);
                            putchar(0xAC);
                    }
                    spkfour++;
              }
              if(temp<five+(four-five)/2-10&&temp>five-(four-five)/2)
              {
                    lcd_pos(2,0);
                    for(int i = 0;i<16;i++)
                    {
                      write_dat(tabfive[i]);
                    }
                    if(spkfive%2==0)
                    {
                      putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x08);
                      putchar(0xBB);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC);
                    }
                    spkfive++;
              }           
            }
         }
         kongstate =1;
      }
  }   
}
int main( void )
{
    WDTCTL = WDTPW + WDTHOLD;
  //  pFlash = (unsigned int *)0x1800;
    /*FCTL3 = FWKEY;
    FCTL1 = FWKEY + ERASE;
    *pFlash = 0;
    while(FCTL3 & BUSY);
    FCTL1 = FWKEY + WRT;
    *pFlash = 100;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;*/
    P1DIR  = BIT4 + BIT5 + BIT6;
    P3DIR  = 0xff;
    LCD_init();
    init_port();
    bsp_Init();
    TA0R=58982;
    TA0CTL|=TACLR+TAIE;
    TA0CTL|=TASSEL_1+MC_2+TAIE;
    _EINT();
   
    /*lcd_pos(0,0);
    for(int i = 0;i<12;i++)
    {
      write_dat(tab0[i]);
    }
    kong=13297;
    jiandao=13070;
    shitou=12864;
    bu=12773;*/
    putchar(0xAA);putchar(0x0B);putchar(0x01);putchar(0x02);putchar(0xB8);
    lcd_pos(0,2);
    for(uchar i=0;i<8;i++)
      write_dat(welcome0[i]);
    lcd_pos(1,1);
    for(uchar i=0;i<12;i++)
      write_dat(welcome1[i]);
                      putchar(0xAA);
                      putchar(0x07);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0x0E);
                      putchar(0xC1);
                     
                      putchar(0xAA);
                      putchar(0x02);
                      putchar(0x00);
                      putchar(0xAC);
   
  while(1)
  {
  temp1 = ReadRegfdc2214(0x00);//读取第一通道的值
//  delay_ms(100);
  temp2 = ReadRegfdc2214(0x01);
 // temp2=0;
  tempand  = temp1*65536 + temp2;
  tempand = tempand/1000;
  tem[0]=tem[1];tem[1]=tem[2];tem[2]=tem[3];tem[3]=tem[4];tem[4]=tem[5];
  tem[5]=tempand;
  temp=(tem[0]+tem[1]+tem[2]+tem[3]+tem[4]+tem[5])/6;
  /*putchar(0xAA);
  putchar(0x07);
  putchar(0x02);
  putchar(0x00);
  putchar(0x04);
  putchar(0xB7);
 
  putchar(0xAA);
  putchar(0x02);
  putchar(0x00);
  putchar(0xAC);*/
 // putchar(0x01);
 // putchar(0xBB);
//   printf("%lu\n",temp);
/*    tabledata[0]=temp/10000+'0';
    tabledata[1]=temp%10000/1000+'0';
    tabledata[2]=temp%1000/100+'0';
    tabledata[3]=temp%100/10+'0';
    tabledata[4]=temp%10+'0';
    tabledata[5]=' ';
             
    lcd_pos(3,4);
    for(uchar i = 0;i<5;i++)
            {
              write_dat(tabledata[i]);
            }

    */
    delay_ms(100);
  }
}

