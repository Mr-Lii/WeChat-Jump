#include <reg52.h>
#include <stdio.h>
#include <LCD12864.h>
#include <key.h>
#include <Interrupt.h>

sbit pwm=P3^7;		//PWM信号输出口

int Juli=0;	//int 范围是-32768—+32767
int Bai,Shi,Ge,AA,BB,CC;
int AnTim=0;	//按住时间
	
unsigned char count;		//0.5ms次数标识,可修改
unsigned char angle;		//角度标识


void main()
{
	Time0_Init();
	LCD12864_Init();	//LCD12864初始化
	
	angle=12;
	count=0;
	
	LCD12864DisplayString(0,0,"跳一跳》物理辅助");
	LCD12864DisplayString(1,0,"输入：");
	LCD12864DisplayString(1,5,"mm");
	LCD12864DisplayString(2,0,"范围：0-999");
//	LCD12864DisplayValue(2,0,val);	//显示距离
	LCD12864DisplayString(3,0,"Master Li");
	LCD12864DisplayPictrue(32,5,48,32,pictrue2);//显示机器猫

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

		LCD12864DisplayValue(1,3,Juli);	//显示距离
//		LCD12864DisplayValue(2,0,val);	//显示键值

		AnTim=Juli*14;	//计算按住时间
		
		while(val==16)	//确定
		{
//			key_scan();
//			LCD12864DisplayValue(2,0,val);	//显示键值
						
			angle=10;	//按下
			count=0;	//按键按下 则20ms周期从新开始
			
			Delay_ms(AnTim);
			
			angle=11;	//抬起
			count=0;	//按键按下 则20ms周期从新开始
				
			ShuWei=0;
			val=0;
		}

	}
}


void Time0_Int() interrupt 1
{
	TH0=0xff;
	TL0=0xa3;
	if(count<angle)		//判断0.5ms次数是否小于角度标识
		pwm=1;		//确实小于，pwm输出高电平
	else
		pwm=0;		//大于则输出低电平
		count=(count+1);		//0.5ms次数加1
		count=count%40;		//次数始终保持为40即保持周期为20ms
}
