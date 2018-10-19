#include "bsp.h"

#define DISABLE_INT   UART_SR=__get_SR_register(); _DINT()
#define RESTORE_INT   if(UART_SR & GIE)            _EINT()
unsigned char UART_SR;

unsigned char UART_TXBUF[UART_TXBUFSIZE];       //����FIFO����
unsigned char UART_RXBUF[UART_RXBUFSIZE];       //����FIFO����
unsigned char UART_TXIndexR = 0;                //����FIFO�Ķ�ָ��
unsigned char UART_TXIndexW = 0;                //����FIFO��дָ��
unsigned char UART_RXIndexR = 0;                //����FIFO�Ķ�ָ��
unsigned char UART_RXIndexW = 0;                //����FIFO��дָ��
unsigned char UART_OutLen = 0;                  //����FIFO�ڴ����͵��ֽ���
unsigned char UART_InLen = 0;                   //����FIFO�ڴ���ȡ���ֽ���

const int UCBRS_Table[8] = {UCBRS_0,UCBRS_1,UCBRS_2,UCBRS_3,UCBRS_4,UCBRS_5,UCBRS_6,UCBRS_7};
/*******************************************************************************
�������ƣ�UART_Init()
�������ܣ�UART��ʼ��
��ڲ�����
  Baud��      ������    (300~115200)
  Parity��    ��żУ��λ('n'=��У��  'p'=żУ��  'o'=��У��)
  DatsBits��  ����λλ��(7��8)
  StopBits��  ֹͣλλ��(1��2)
���ڲ���������1��ʾ��ʼ���ɹ�������0��ʾ��ʼ��ʧ��
Ӧ�÷�����UART_Init(9600,'n',8,1);
����˵����
*******************************************************************************/
char UART_Init(long int Baud,char Parity,char DataBits,char StopBits)
{
  unsigned long int fSMCLK = 4000000;         //���ڵ�ʱ��Դѡ����Ǹ���ʱ��,���Դ˴���ֵҪ��ʵ�ʵĸ���ʱ�Ӷ��ı�
  unsigned int UCBRS_Index;
  UART_TXD_SEL |= UART_TXD_PIN;               //����IO��
  UART_RXD_SEL |= UART_RXD_PIN;
  UCAxCTL1 |= UCSWRST;
  
  UCAxCTL0 &=~(UCPEN+UCPAR+UC7BIT+UCSPB);     //���֮ǰ����
  //------------------------------���ò�����------------------------------
  if (Baud > 115200)  return (0);
  else if (Baud >= 9600)
  {
    UCAxCTL1 |= UCSSEL__SMCLK;
    UCAxBRW = fSMCLK/Baud;
    UCBRS_Index = fSMCLK*8/Baud - 8*UCA1BRW;
  }
  else if (Baud >= 300)
  {
    UCAxCTL1 |= UCSSEL__ACLK;
    UCAxBRW = F_ACLK/Baud;
    UCBRS_Index = F_ACLK*8/Baud - 8*UCA1BRW;
  }
  else return (0);
  //------------------------------����У��λ------------------------------
  switch (Parity)
  {
  case 'n':case 'N':  UCAxCTL0 &=~UCPEN;break;          //��У��
  case 'p':case 'P':  UCAxCTL0 |= UCPEN+UCPAR;break;    //żУ��
  case 'o':case 'O':  UCAxCTL0 |= UCPEN;break;          //��У��
  default:  return (0);                                 //��������
  }
  //------------------------------��������λ------------------------------
  switch (DataBits)
  {
  case 7:case '7':    UCAxCTL0 |= UC7BIT;break;         //7λ����λ
  case 8:case '8':    UCAxCTL0 &=~UC7BIT;break;         //8λ����λ
  default:  return (0);                                 //��������
  }
  //------------------------------����ֹͣλ------------------------------
  switch (StopBits)
  {
  case 1:case '1':    UCAxCTL0 &=~UCSPB;break;          //1λֹͣλ
  case 2:case '2':    UCAxCTL0 |= UCSPB;break;          //2λֹͣλ
  default:  return (0);                                 //��������
  }
   
  UCAxMCTL = UCBRS_Table[UCBRS_Index]+UCBRF_0;
  UCAxCTL1 &= ~UCSWRST;
  UCAxIE |= UCRXIE;           //���������ж�
  
  return (1);                 //��ʼ���ɹ�
}

/*******************************************************************************
�������ƣ�UART_PutChar()
�������ܣ�UART ����һ���ֽ�
��ڲ�����
  Chr��   �����͵�һ�ֽ�����
���ڲ���������1��ʾ���ͳɹ�������0��ʾ����ʧ��
Ӧ�÷�����UART_PutChar('a');
����˵�������͹����У�������CPU����
*******************************************************************************/
char UART_PutChar(unsigned char Chr)
{
  if (UART_OutLen == UART_TXBUFSIZE)  return(0);
  if (UART_OutLen == 0)  UCAxIFG |= UCTXIFG; //��Ϊ��һ���ֽ�,��Ϊ�����һ�η����ж�
  DISABLE_INT;
  UART_OutLen++;
  UART_TXBUF[UART_TXIndexW] = Chr;          //��������ͨ��дָ��д��FIFO
  if (++UART_TXIndexW >= UART_TXBUFSIZE)  UART_TXIndexW = 0;
  RESTORE_INT;
  UCAxIE |= UCTXIE;                         //���������ж�
  return (1);
}

/*******************************************************************************
�������ƣ�UART_GetChar()
�������ܣ�UART ��ȡһ���ֽ�
��ڲ�����
  *Chr��  ��ȡ��������ŵĵ�ַָ��
���ڲ���������1��ʾ��ȡ�ɹ�������0��ʾ��ȡʧ��
Ӧ�÷�����if (ch = UART_GetChar()) ...;
����˵������ȡ�����У�������CPU����
*******************************************************************************/
char UART_GetChar(unsigned char *Chr)
{
  if (UART_InLen == 0) return (0);          //��FIFO�������ݣ�����0
  DISABLE_INT;
  UART_InLen--;
  *Chr = UART_RXBUF[UART_RXIndexR];         //ͨ����ָ���FIFO�ж�ȡ���ݲ�����
  if (++UART_RXIndexR >= UART_RXBUFSIZE)  UART_RXIndexR = 0;
  RESTORE_INT;
  return (1);
}

/*******************************************************************************
�������ƣ�UART_WaitChar()
�������ܣ�UART �ȴ�һ���ֽ�
��ڲ�����
���ڲ�����
Ӧ�÷�����
����˵����
*******************************************************************************/
char UART_WaitChar()
{ 
//  DIR485_IN;
//  while((IFG1&URXIFG0)==0)WDTCTL=WDT_ARST_1000;//ι�� 
//  return(RXBUF0);
  return (1);
}

/*******************************************************************************
�������ƣ�UART_GetCharsInRxBuf()
�������ܣ���ȡFIFO���ѽ��յ������ֽ���
��ڲ�������
���ڲ�����FIFO���ѽ��յ��������ֽ���
*******************************************************************************/
unsigned int UART_GetCharsInRxBuf(void)
{
  return (UART_InLen);
}

/*******************************************************************************
�������ƣ�UART_ClrRxBuf()
�������ܣ���ս���FIFO
��ڲ�������
���ڲ�������
*******************************************************************************/
void UART_ClrRxBuf(void)
{
  DISABLE_INT;
  UART_InLen = 0;
  UART_RXIndexR = 0;
  UART_RXIndexW = 0;
  RESTORE_INT;
}

/*******************************************************************************
�������ƣ�UART_IsTxEmpty()
�������ܣ��жϷ���FIFO�Ƿ�Ϊ��
��ڲ�������
���ڲ���������1��ʾ����FIFOΪ�գ�����0��ʾ����FIFO�ǿ�
*******************************************************************************/
char UART_IsTxEmpty(void)
{
  if (UART_OutLen == 0)  return(1);
  else  return(0);
}


/*******************************************************************************
�������ƣ�UART_ISR()
�������ܣ�UART ����/�����жϷ������
*******************************************************************************/
#pragma vector=USCI_Ax_VECTOR
__interrupt void UART_Ax_ISR(void)
{
  switch(__even_in_range(UCAxIV,4))
  {
  case 0:break;                 // Vector 0 - no interrupt
  case 2:                       // Vector 2 - RXIFG
    UART_InLen++;
    UART_RXBUF[UART_RXIndexW] = UCAxRXBUF;    //��������ͨ��дָ��д��FIFO
    if (++UART_RXIndexW >= UART_RXBUFSIZE)  UART_RXIndexW = 0;
    break;
  case 4:                       // Vector 4 - TXIFG
    if (UART_OutLen > 0)                      //��FIFO���д����͵�����
    {
      UART_OutLen--;
      UCAxTXBUF = UART_TXBUF[UART_TXIndexR];  //ͨ����ָ���FIFO�ж�ȡ���ݲ�����
      while (!(UCAxIFG & UCTXIFG));
      if (++UART_TXIndexR >= UART_TXBUFSIZE)  UART_TXIndexR = 0;
    }
    else UCAxIE &=~UCTXIE;                    //���ݷ�����ϣ��رշ����ж�
    break;
  default: break;
  }  
}

/*===================�����Ǵ����ն��豸�ӿں�����==========================*/

#define LINE_LENGTH 20          /* �л�������С������ÿ�����������ַ���*/

/*��׼�ն��豸�У�����ASCII�붨�壬�����޸�*/
#define In_BACKSP 0x08          /* ASCII  <--  (�˸��)  */
#define In_DELETE 0x7F          /* ASCII <DEL> (DEL ��)  */
#define In_EOL '\r'             /* ASCII <CR>  (�س���)  */
#define In_SKIP '\3'            /* ASCII control-C */
#define In_EOF '\x1A'           /* ASCII control-Z */

#define Out_DELETE "\x8 \x8"    /* VT100 backspace and clear */
#define Out_SKIP "^C\n"         /* ^C and new line */
#define Out_EOF "^Z"            /* ^Z and return EOF */
#include "stdio.h"

/****************************************************************************
�������ƣ�putchar()
�������ܣ����׼�ն��豸����һ�ֽ�����(1��ASCII�ַ�)
��ڲ�����
  ch��    �����͵��ַ�  
���ڲ������������ַ�
����˵����printf��������øú�����Ϊ�ײ����������Ӵ�������ַ���PC���ĳ����ն�
          ����ϣ�printf�Ľ������ӡ�������ն��ϡ����޸ĸú��������ַ���������ʽ
          ���������ʾ��LCD�ϣ�printf�Ľ������ʾ��LCD�ϡ�
****************************************************************************/
int putchar(int ch)
{
  if (ch == '\n')        //  '\n'(�س�)��չ�� '\n''\r' (�س�+����) 
  {
    UART_PutChar(0x0d) ; //'\r'
  }
  UART_PutChar(ch);      //�Ӵ��ڷ�������
  return (ch);
}
/****************************************************************************
�������ƣ�put_message()
�������ܣ����׼�ն��豸����һ���ַ���
��ڲ�����
  *s��    �ַ���(����)ͷָ��(������)
���ڲ�������
****************************************************************************/
static void put_message(char *s)
{
  while (*s)        //��ǰ�ַ���Ϊ�� (�ַ�����0x00��β)
    putchar(*s++);  //���һ���ַ���ָ��ָ����һ�ַ�
}

/****************************************************************************
�������ƣ�getchar()
�������ܣ��ӱ�׼�ն��豸����һ�ֽ�����(1��ASCII�ַ�)
��ڲ�������
���ڲ������յ����ַ�
����˵����scanf��������øú�����Ϊ�ײ����롣�����PC�����̽��������ն����ͨ��
          ���������ַ�����Ƭ���ϡ�scanf���������뼴Դ��PC�����̡����޸ĸú�����
          ���ַ���������ʽ���룬�絥Ƭ��IO�ڣ����ð�ť��scanf�����������ݡ�����
          �����л��棬�ܹ������˸��ɾ��������������Ҫɾ����������ֱ�ӵ���
          UART_GetChar()������
****************************************************************************/
int getchar(void)
{
  static char io_buffer[LINE_LENGTH + 2];     /* Where to put chars */
  static int ptr;                             /* Pointer in buffer */
  unsigned char c;

  for (;;)
  {
    if (io_buffer[ptr])
      return (io_buffer[ptr++]);
    ptr = 0;
    for (;;)
    {
      if (UART_GetChar(&c))
      {
        if (c == In_EOF && !ptr)
        {
          put_message(Out_EOF);
          return EOF;
        }
        if ((c == In_DELETE)||(c==In_BACKSP))
        {
          if (ptr)
          {
            ptr--;
            put_message(Out_DELETE);
          }
        }
        else if (c == In_SKIP)
        {
          put_message(Out_SKIP);
          ptr = LINE_LENGTH + 1;  /* Where there always is a zero... */
          break;
        }
        else if (c == In_EOL)
        {
          putchar(io_buffer[ptr++] = '\n');
          io_buffer[ptr] = 0;
          ptr = 0;
          break;
        }
        else if (ptr < LINE_LENGTH)
        {
          if (c >= ' ')
          {
            putchar(io_buffer[ptr++] = c);
          }
        }
        else
        {
          putchar('\7');
        }
      }
    }
  }
}