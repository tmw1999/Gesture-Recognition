#include "bsp.h"

/*******************************************************************************
* ������	: WriteRegfdc2214 
* ����	    : д�Ĵ�������
* �������  : add��value
* ���ز���  : ��
*******************************************************************************/
void WriteRegfdc2214(unsigned char add,unsigned int value)
{
    IIC_Start(); 
    IIC_Send_Byte(0X54);	 //ADDR=0ʱ����ַ0X2A<<1+0=0X54  
    IIC_Wait_Ack();          //��Ӧ��
    IIC_Send_Byte(add);      //д��ַ
    IIC_Wait_Ack();          //��Ӧ��
    IIC_Send_Byte(value>>8); //д��8λ
    IIC_Wait_Ack();
    IIC_Send_Byte(value&0xff);//д��8λ
    IIC_Wait_Ack();
    IIC_Stop();               //����һ��ֹͣ���� 
    delay_ms(10);	 
}
/*******************************************************************************
* ������	: ReadRegLDC 
* ����	    : ���Ĵ�������
* �������  : add��ַ
* ���ز���  : ��
*******************************************************************************/
unsigned int ReadRegfdc2214(unsigned char add)
{
    unsigned int status;
    unsigned int a,b;
    IIC_Start(); 
    IIC_Send_Byte(0X54);	   //д����ADDR=0
    IIC_Wait_Ack();
    IIC_Send_Byte(add);     //��ַ
    IIC_Wait_Ack();
    IIC_Start();            //���¿�ʼ
    IIC_Send_Byte(0X55);	   //���Ͷ�����ADDR=0
    IIC_Wait_Ack(); 
    a=IIC_Read_Byte(1);     //����λ
    b=IIC_Read_Byte(0);		 //����λ
    status=(a<<8)+b;
    IIC_Stop();
    return (status);
}
/*******************************************************************************
* ������	: InitSingleLDC1314 
* ����	    : ��ʼ����ͨ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitSinglefdc2214(void)
{
    WriteRegfdc2214(0x08,0xFFFF);//����ת��ʱ��  ���ôﵽ��߾���

    WriteRegfdc2214(0x10,0x0064);//FDC2214_SETTLECOUNT_CH0 ����

    WriteRegfdc2214(0x14,0x2001); //��Ƶϴϵ��
    //    LDC_Write_Reg(0x0C,0x0F00); //����
    WriteRegfdc2214(0x19,0x0000); //ERROE_CONFIG
	
    WriteRegfdc2214(0x1B,0x020D); //ͨ������
    WriteRegfdc2214(0x1E,0xF800); //������������
    WriteRegfdc2214(0x1A,0x1C81); //��������
}
/*******************************************************************************
* ������	: InitMultiLDC1314 
* ����	    : ��ʼ����ͨ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitMultifdc2214(void)//˫ͨ��
{
    WriteRegfdc2214(0x08,0x04D6);//ת��ʱ��
  	WriteRegfdc2214(0x09,0x04D6);
	WriteRegfdc2214(0x0A,0x04D6);//ת��ʱ��
  	WriteRegfdc2214(0x0B,0x04D6);
	
	WriteRegfdc2214(0x0C,0x0F00); //��0ֵ 	CH0
	WriteRegfdc2214(0x0D,0x0F00); //��0ֵ   CH1
	WriteRegfdc2214(0x0E,0x0F00); //��0ֵ   CH2
	WriteRegfdc2214(0x0F,0x0F00); //��0ֵ   CH3
  	
	WriteRegfdc2214(0x10,0x000A);//����ʱ�� CH1 
  	WriteRegfdc2214(0x11,0x000A);//CH2
	WriteRegfdc2214(0x12,0x000A);//CH3
	WriteRegfdc2214(0x13,0x000A);//CH4
  	
	WriteRegfdc2214(0x14,0x2002);//��Ƶ
  	WriteRegfdc2214(0x15,0x2002);
    WriteRegfdc2214(0x16,0x2002);//��Ƶ
  	WriteRegfdc2214(0x17,0x2002);
	
  	WriteRegfdc2214(0x19,0x0000);
  	WriteRegfdc2214(0x1B,0xC20D);//���ö�ͨ��   2ͨ��--0x820D
	
  	WriteRegfdc2214(0x1E,0x9000); //�������� CH0
  	WriteRegfdc2214(0x1F,0x9000); //CH1
	WriteRegfdc2214(0x20,0x9000); //CH2
  	WriteRegfdc2214(0x21,0x9000); //CH3
		  
//  	LDC_Write_Reg(0x1A,0x1401);


	WriteRegfdc2214(0x1A,0x1C81);//���üĴ���
	
}
	//Configuration register
	//	Active channel Select: b00 = ch0; b01 = ch1; b10 = ch2; b11 = ch3;
	//  |Sleep Mode: 0 - device active; 1 - device in sleep;
	//  ||Reserved, reserved, set to 1
	//  |||Sensor Activation Mode: 0 - drive sensor with full current. 1 - drive sensor with current set by DRIVE_CURRENT_CHn 
	//  ||||Reserved, set to 1
	//  |||||Reference clock: 0 - use internal; 1 - use external clock
	//  ||||||Reserved, set to 0
	//  |||||||Disable interrupt. 0 - interrupt output on INTB pin; 1 - no interrupt output
	//  ||||||||High current sensor mode: 0 - 1.5mA max. 1 - > 1.5mA, not available if Autoscan is enabled
	//  |||||||||     Reserved, set to 000001
	//  |||||||||     |
	// CCS1A1R0IH000000 -> 0001 1100 1000 0001 -> 0x1E81      0001 1100 1000 0001 -> 0x1C81


