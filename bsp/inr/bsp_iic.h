#ifndef __BSP_IIC_H_
#define __BSP_IIC_H_
//IO��������

#define IIC_SDA_IN()   P4DIR &=~ BIT1           //SDA����Ϊ����ģʽ
#define IIC_SDA_OUT()  P4DIR |= BIT1            //SDA����Ϊ���ģʽ

//IO��������
#define IIC_SCL_L      P4OUT &=~ BIT2           //����SCLΪ�͵�ƽ
#define IIC_SCL_H      P4OUT |= BIT2            //����SCLΪ�ߵ�ƽ
#define IIC_SDA_L      P4OUT &=~ BIT1           //����SDAΪ�͵�ƽ
#define IIC_SDA_H      P4OUT |= BIT1            //����SDAΪ�ߵ�ƽ
#define IIC_READ_SDA   P4IN&BIT1                //����SDA


//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				        //����IIC��ʼ�ź�
void IIC_Stop(void);	  			      //����IICֹͣ�ź�
void IIC_Send_Byte(unsigned char txd);			    //IIC����һ���ֽ�
unsigned char IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC_Wait_Ack(void); 				      //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					        //IIC����ACK�ź�
void IIC_NAck(void);				        //IIC������ACK�ź�

#endif