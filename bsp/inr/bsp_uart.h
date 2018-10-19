#ifndef __BSP_UART_H_
#define __BSP_UART_H_

#include "stdio.h"

//���²�����Ҫ�û�����
#define UART_TXBUFSIZE      64                //����FIFO����
#define UART_RXBUFSIZE      16                //����FIFO����
#define UART_MODULE         UART_UCA1         //SPIͨ�Žӿ�ѡ�õ�ģ��

//���²����û������޸�
#define F_ACLK              (32768)
#define UART_UCA0           (0)
#define UART_UCA1           (1)

#if   (UART_MODULE == UART_UCA0)
  #define UART_TXD_PORT       P3OUT           //����TXD  ->  P3.3
  #define UART_TXD_DIR        P3DIR
  #define UART_TXD_SEL        P3SEL
  #define UART_TXD_PIN        BIT3
  
  #define UART_RXD_PORT       P3OUT           //����RXD  ->  P3.4
  #define UART_RXD_DIR        P3DIR
  #define UART_RXD_SEL        P3SEL
  #define UART_RXD_PIN        BIT4

  #define USCI_Ax_VECTOR      USCI_A0_VECTOR
  #define UCAxCTL0            UCA0CTL0
  #define UCAxCTL1            UCA0CTL1
  #define UCAxBRW             UCA0BRW
  #define UCAxMCTL            UCA0MCTL
  #define UCAxIFG             UCA0IFG
  #define UCAxIE              UCA0IE
  #define UCAxIV              UCA0IV
  #define UCAxTXBUF           UCA0TXBUF
  #define UCAxRXBUF           UCA0RXBUF
  #define UCAxSTAT            UCA0STAT
#elif (UART_MODULE == UART_UCA1)
  #define UART_TXD_PORT       P4OUT           //����TXD  ->  P4.4
  #define UART_TXD_DIR        P4DIR
  #define UART_TXD_SEL        P4SEL
  #define UART_TXD_PIN        BIT4
  
  #define UART_RXD_PORT       P4OUT           //����RXD  ->  P4.5
  #define UART_RXD_DIR        P4DIR
  #define UART_RXD_SEL        P4SEL
  #define UART_RXD_PIN        BIT5

  #define USCI_Ax_VECTOR      USCI_A1_VECTOR
  #define UCAxCTL0            UCA1CTL0
  #define UCAxCTL1            UCA1CTL1
  #define UCAxBRW             UCA1BRW
  #define UCAxMCTL            UCA1MCTL
  #define UCAxIFG             UCA1IFG
  #define UCAxIE              UCA1IE
  #define UCAxIV              UCA1IV
  #define UCAxTXBUF           UCA1TXBUF
  #define UCAxRXBUF           UCA1RXBUF
  #define UCAxSTAT            UCA1STAT
#endif

//�ⲿ��������
extern char UART_Init(long int Baud,char Parity,char DataBits,char StopBits);
extern char UART_PutChar(unsigned char Chr);
extern char UART_GetChar(unsigned char *Chr);
extern unsigned int UART_GetCharsInRxBuf(void);
extern void UART_ClrRxBuf(void);
extern char UART_IsTxEmpty(void);

#endif