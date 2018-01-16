void Time0_Init()		//定时器初始化
{
	TMOD=0x01;		//定时器0工作在方式1
	IE=0x82;
	TH0=0xff;
	TL0=0xa3;		//11.0592MHz晶振，0.5ms
	TR0=1;		//定时器开始
}
