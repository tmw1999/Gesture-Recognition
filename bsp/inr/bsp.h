
#ifndef __BSP_H_
#define __BSP_H_

#include<msp430f5529.h>
#include "bsp_sysclk.h"
#include "bsp_uart.h"
#include "bsp_iic.h"
#include "bsp_fdc2214.h"

#define F_CPU  8000000        //���ݵ�ǰ��Ƶ(MCLK)���㣬��֤��ʱ��������Ƶ�仯(��������430�ڲ��Դ�����ʱ���ܾ�׼)
#define delay_us(A)  __delay_cycles( F_CPU/1000000.0 *A )
#define delay_ms(A)  __delay_cycles( F_CPU/1000.0 *A )
#define delay_s(A)   __delay_cycles( F_CPU/1.0*A )




extern void bsp_Init(void);
#endif 