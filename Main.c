#include <reg52.h>
#include <stdio.h>
#include <LCD12864.h>
#include <key.h>
#include <Interrupt.h>

sbit pwm=P3^7;		//PWM�ź������

int Juli=0;	//int ��Χ��-32768��+32767
int Bai,Shi,Ge,AA,BB,CC;
int AnTim=0;	//��סʱ��
	
unsigned char count;		//0.5ms������ʶ,���޸�
unsigned char angle;		//�Ƕȱ�ʶ


void main()
{
	Time0_Init();
	LCD12864_Init();	//LCD12864��ʼ��
	
	angle=12;
	count=0;
	
	LCD12864DisplayString(0,0,"��һ����������");
	LCD12864DisplayString(1,0,"���룺");
	LCD12864DisplayString(1,5,"mm");
	LCD12864DisplayString(2,0,"��Χ��0-999");
//	LCD12864DisplayValue(2,0,val);	//��ʾ����
	LCD12864DisplayString(3,0,"Master Li");
	LCD12864DisplayPictrue(32,5,48,32,pictrue2);//��ʾ����è

	while(1)
	{
//		P0=sign[val];
        key_scan();
		if(val<=9)
		{
			switch(ShuWei)
			{
				case 1:
					Bai=val;
				break;
				
				case 2:
					Shi=val;
				break;
			
				case 3:
					Ge=val;
				break;
			}

			switch(ShuWei)
			{
				case 1:
					Juli=Bai;
				break;
			
				case 2:
					Juli=Bai*10+Shi;
				break;
			
				case 3:
					Juli=Bai*100+Shi*10+Ge;
					if(Juli>999) ShuWei=0;
				break;
			}
		}
		
		if(ShuWei==0)
		{
			Juli=0;
		}

		LCD12864DisplayValue(1,3,Juli);	//��ʾ����
//		LCD12864DisplayValue(2,0,val);	//��ʾ��ֵ

		AnTim=Juli*14;	//���㰴סʱ��
		
		while(val==16)	//ȷ��
		{
//			key_scan();
//			LCD12864DisplayValue(2,0,val);	//��ʾ��ֵ
						
			angle=10;	//����
			count=0;	//�������� ��20ms���ڴ��¿�ʼ
			
			Delay_ms(AnTim);
			
			angle=11;	//̧��
			count=0;	//�������� ��20ms���ڴ��¿�ʼ
				
			ShuWei=0;
			val=0;
		}

	}
}


void Time0_Int() interrupt 1
{
	TH0=0xff;
	TL0=0xa3;
	if(count<angle)		//�ж�0.5ms�����Ƿ�С�ڽǶȱ�ʶ
		pwm=1;		//ȷʵС�ڣ�pwm����ߵ�ƽ
	else
		pwm=0;		//����������͵�ƽ
		count=(count+1);		//0.5ms������1
		count=count%40;		//����ʼ�ձ���Ϊ40����������Ϊ20ms
}
