//#include <intrins.h>


void Delay_ms(unsigned int t)	//��ʱms������unsigned int ��Χ��0-65535
{
	unsigned int i,j;
	for(i=t;i>0;i--)
	for(j=0;j<114;j++);	//11.0592MHz����:114��	12MHz����123
}


////������ms��Ҫ��ʱ��ms�����Զ���Ӧ��ʱ��
//#define MAIN_Fosc	11059200UL    //������ʱ��HZ

//typedef unsigned int uint;

//void Delay_ms(uint ms)	//uint�ȼ���unsigned int
//{
//	uint i;
//	do
//	{
//		i=MAIN_Fosc/9600;
//		while(--i);	//96T per loop
//    }
//	while(--ms);	//--ms  ms=ms-1
//}


////5us ��ʱ�������Զ���Ӧ��ʱ��
//#define MAIN_Fosc	11059200UL	//������ʱ��HZ

//void delay5us()
//{
//	#if MAIN_Fosc==11059200
//		_nop_();
//	#elif MAIN_Fosc == 12000000
//		_nop_();
//	#elif MAIN_Fosc == 22118400
//		_nop_();
//		_nop_();
//		_nop_();
//	#elif
//}
