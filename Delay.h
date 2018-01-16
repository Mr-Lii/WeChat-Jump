//#include <intrins.h>


void Delay_ms(unsigned int t)	//延时ms函数，unsigned int 范围是0-65535
{
	unsigned int i,j;
	for(i=t;i>0;i--)
	for(j=0;j<114;j++);	//11.0592MHz晶振:114；	12MHz晶振：123
}


////参数：ms，要延时的ms数，自动适应主时钟
//#define MAIN_Fosc	11059200UL    //定义主时钟HZ

//typedef unsigned int uint;

//void Delay_ms(uint ms)	//uint等价于unsigned int
//{
//	uint i;
//	do
//	{
//		i=MAIN_Fosc/9600;
//		while(--i);	//96T per loop
//    }
//	while(--ms);	//--ms  ms=ms-1
//}


////5us 延时函数，自动适应主时钟
//#define MAIN_Fosc	11059200UL	//定义主时钟HZ

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
