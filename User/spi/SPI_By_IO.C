
#include "SPI_By_IO.h"
//#include "usart.h"
#include "config.h"

void SPI_By_IO_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(SPI_GPIO_Port_RCC, ENABLE);	

    //SPI_CS
    GPIO_InitStructure.GPIO_Pin = SPI_CS_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStructure);
    SPI_CS_1();											//cs��ʼ��ƽΪ��

    //SPI_CLK	
    GPIO_InitStructure.GPIO_Pin = SPI_CLK_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(SPI_CLK_GPIO_Port, &GPIO_InitStructure);
    SPI_CLK_1();										//CLK��ʼ��ƽΪ��

    //SPI_MISO	
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStructure);

    //SPI_MOSI	
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(SPI_MOSI_GPIO_Port, &GPIO_InitStructure);
    SPI_MOSI_1();									//MOSI��ʼ��ƽΪ��
		
		//AD_SYNC	
//    GPIO_InitStructure.GPIO_Pin = AD7175_SYNC_Pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(AD7175_SYNC_Port, &GPIO_InitStructure);
//		//AD_SYNC_0();									//SYNC�͵�ƽ�������µ�ת��
//		AD_SYNC_1();		
}
void Reset64CLK(void)
{
		u8 i=0;
		for(i=0;i<65;i++)
	{
		SPI_CLK_0();					//��ʼ״̬ʱ��Ϊ�ߵ�ƽ
		SPI_CLK_Delay();
		SPI_CLK_1();					//��ʼ״̬ʱ��Ϊ�ߵ�ƽ
		SPI_CLK_Delay();
	}
}
void SPI_Send_One_Byte(u8 data)
{		u8 i=0;
    SPI_CS_0();
    SPI_CLK_Delay();
		for(i=0;i<8;i++)
		{
			SPI_CLK_1();					//��ʼ״̬ʱ��Ϊ�ߵ�ƽ
			SPI_CLK_Delay();
			
			SPI_CLK_0();					//CLK�½��أ�������·�ɸı�״̬
			if( data & 0x80 )			//MSB_First
				{	SPI_MOSI_1();}
			else {	SPI_MOSI_0();}
			SPI_CLK_Delay();

			SPI_CLK_1();					//�����أ����ݴ�����ӻ�
			data=data<<1;					//�������������ƣ��Է�����һλ����
		}
    SPI_CLK_Delay();			//һ���ֽ����ݷ�����ϣ�����ʱ
    SPI_CS_0();
    SPI_CLK_Delay();
}
//u8 SPI_Read_One_Byte(void)
//{
//		u8 i=0;
//		u8 dat=0;
//		SPI_CLK_1();	
//		SPI_CLK_Delay();
//	
//		for(i=0;i<8;i++)
//		{
//			SPI_CLK_0();
//			dat=dat<<1;						//���ݻ���������ƣ���׼��������һbit����
//							//��ʼ״̬��Ϊ�ߵ�ƽ
//			
//			
//								//CLK�½��أ��ӻ����Ըı�������·��ƽ
//			SPI_CLK_Delay();			//�͵�ƽ����
//			
//					//�����أ����ݴ�����ӻ�
//		
//			if( Read_SPI_MISO()	)	//����ȡ������Ϊ1
//				{	dat= dat|0x01;}		//��
//				SPI_CLK_1();	
//		}	
//			
//		
//		return	dat;						//���ؽ��յ��Ľ������
//}
u8 SPI_Read_One_Byte(void)
{
    SPI_CS_0();
    SPI_CLK_Delay();
		u8 i=0;
		u8 dat=0;

		for(i=0;i<8;i++)
		{
			dat=dat<<1;						//���ݻ���������ƣ���׼��������һbit����
			SPI_CLK_1();					//��ʼ״̬��Ϊ�ߵ�ƽ
			SPI_CLK_Delay();
			
			SPI_CLK_0();					//CLK�½��أ��ӻ����Ըı�������·��ƽ
			SPI_CLK_Delay();			//�͵�ƽ����
			
			SPI_CLK_1();					//�����أ����ݴ�����ӻ�
			SPI_CLK_Delay();			//����ʱ���ٶ�ȡ����
			if( Read_SPI_MISO()	)	//����ȡ������Ϊ1
				{	dat= dat|0x01;}		//�洢
		}
			SPI_CLK_Delay();
        SPI_CS_1();
		return	dat;						//���ؽ��յ��Ľ������
}

