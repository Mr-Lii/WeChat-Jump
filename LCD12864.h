#include <Delay.h>

sbit E=P3^4;	//脉冲使能
sbit RW=P3^3;	//读写选择
sbit RS=P3^2;	//数据命令选择
sbit rst=P3^5;	//12864复位

//char code sign[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};



unsigned char code pictrue2[192]={	//图片：机器猫48*32
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0X80,0X00,0X00,0X00,0X1F,0XFF,
0XFE,0X00,0X00,0X00,0X7F,0X1F,0XFF,0X80,0X00,0X01,0XFC,0X0C,0X3F,0XE0,0X00,0X07,
0XF8,0X48,0X1F,0XF8,0X00,0X0F,0X98,0XD1,0X8F,0XFC,0X00,0X1C,0X4C,0XF3,0X9F,0XFE,
0X00,0X1A,0X33,0X39,0XB3,0XFE,0X00,0X33,0XE3,0XFC,0X40,0XFE,0X00,0X31,0X81,0XE1,
0X9E,0X7F,0X00,0X10,0XC1,0X03,0XC0,0X3E,0X00,0X18,0X01,0X03,0X70,0X9E,0X00,0X1F,
0X03,0X03,0X83,0X9E,0X00,0X09,0XE2,0X00,0X3E,0X1C,0X00,0X04,0X3F,0X80,0XFF,0X3C,
0X00,0X07,0X01,0XFF,0XC0,0X70,0X00,0X0F,0XC0,0X00,0X00,0XE0,0X00,0X1F,0XF8,0X00,
0X03,0X80,0X0F,0X0C,0X3F,0XE0,0X1E,0X00,0X39,0XED,0X3B,0XFF,0XFF,0X00,0X20,0X7A,
0XFE,0X07,0XFF,0X80,0X60,0X1C,0X1C,0X07,0XFF,0X80,0X60,0X0F,0X03,0XF7,0XFF,0X80,
0X20,0X0F,0XDE,0X3F,0XFF,0X80,0X30,0X0F,0XF0,0X0F,0XFF,0XA0,0X1C,0X0F,0XE0,0X07,
0XF8,0X30,0X0F,0XFF,0XE0,0X03,0XC8,0X60,0X01,0XE0,0X20,0X03,0X87,0XC0,0X00,0X00,
0X30,0X07,0X00,0X00,0X00,0X00,0X0C,0X0C,0X00,0X00,0X00,0X00,0X07,0XF0,0X00,0X00,
};


void Check12864State(void)	//LCD12864检查状态函数
{
	P2=0xff;
	E=0;	//读状态前三控制线的状态
	RS=0;
	RW=1;
	E=1;	//拉高，读状态
	while((P2&0x80)==0x80);	//等待空闲
	E=0;	//写命令后三控制线的状态
	RS=1;
	RW=0;
}


void Write12864Command( unsigned char com)	//LCD12864写命令函数
{ 
	Check12864State();	//检查状态
	P2=com;	//赋值
	E=0;	//写命令前三控制线的状态
	RS=0;
	RW=0;
	E=1;	//拉高，写命令
	E=0;	//写命令后三控制线的状态
	RS=1;
	RW=1;
}
 

void Write12864Data( unsigned char dat)	//LCD12864写数据函数
{
	Check12864State();	//检查状态
	P2=dat;	//赋值
	E=0;	//写数据前三控制线的状态
	RS=1;
	RW=0;
	E=1;	//拉高，写数据
	E=0;	//写数据后三控制线的状态
	RS=0;
	RW=1;
}
 

void LCD12864DisplayValue( unsigned char y,unsigned char x,int v)	//在指定的位置显示变量值函数，unsigned char 范围是256
//y-行数值0-3，x-列数值0-7，v-变量
{
	unsigned char row;
	Write12864Command(0x30);	//基本指令
	Write12864Command(0x06);	//地址计数器自动加以，光标右移
	
	switch(y)	//根据行号选择行地址
	{
		case 0:row=0x80;break;	//第一行首地址
		case 1:row=0x90;break;	//第二行首地址
		case 2:row=0x88;break;	//第三行首地址
		case 3:row=0x98;break;	//第四行首地址
		default:;
	}
	
	Write12864Command(row+x);	//写地址
	Write12864Data(0x30+v/100);	//百位，先将数字化成ASCII码，再写入
	Write12864Data(0x30+v%100/10);	//十位/先将数字化成ASCII码，再写入
	Write12864Data(0x30+v%10);	//个位，先将数字化成ASCII码，再写入
	
}    


void LCD12864DisplayString( unsigned char y,unsigned char x, unsigned char *pstr)	//在指定的位置显示字符串(汉字和ASCII码字符)函数
//y-行数值0-3，x-列数值0-7，pstr-字符串指针
//12864可以显示32个汉字（四行每行8个），一个地址对应一个汉字
//可以显示64个ASCII码字符（四行每行16个），一个地址对应两个字符
//为了实现自动换行功能，这个函数比较繁琐
{
	unsigned char row,n=0;
	Write12864Command(0x30);	//基本指令
	Write12864Command(0x06);	//地址计数器自动加以，光标右移
	switch(y)	//根据行号选择行地址
	{
		case 0:row=0x80;break;	//第一行首地址
		case 1:row=0x90;break;	//第二行首地址
		case 2:row=0x88;break;	//第三行首地址
		case 3:row=0x98;break;	//第四行首地址
		default:;
	}
	Write12864Command(row+x);	//写地址
	while(*pstr!='\0')
	{
		Write12864Data(*pstr);	//写字符
		pstr++;
		n++;	//计数
		if((n+x*2)==16)	//如果一行写完，继续写第二行
		{
			if(y==0) Write12864Command(0x90);	//写下一行地址
			else if(y==1) Write12864Command(0x88);	//写下一行地址
			else if(y==2) Write12864Command(0x98);	//写下一行地址
			else ;
		}
		else if((n+x*2)==32)	//如果第二行写完，继续写第三行
		{
			if(y==0) Write12864Command(0x88);	//写下一行地址
			else if(y==1) Write12864Command(0x98);	//写下一行地址
			else ;
		}
		else if((n+x*2)==48)	//如果第三行写完，继续写第四行
		{
			if(y==0) Write12864Command(0x98);	//写下一行地址
			else ;
		}
		else ;
	}
}


void Clear12864Screen()	//图片模式清屏函数
{
	unsigned char i,j;
	Write12864Command(0x34);	//功能设定：8位控制方式，使用扩充指令
	Write12864Command(0x36);//使用扩充指令，绘图显示控制
	for(i=0;i<32;i++)
	//ST7920可控制256*32点阵（32行256列），而12864液晶实际的行地址只有0-31行,
	//12864液晶的32-63行的行是0-31行地址从第128列划分一半出来的,所以分为上下两半屏，
	//也就是说第0行和第32行同属一行，行地址相同;第1行和第33行同属一行，以此类推
   {                           
		Write12864Command(0x80|i);	//写行地址（垂直地址）
		Write12864Command(0x80);	//写列地址（水平地址）
		for(j=0;j<32;j++)
		Write12864Data(0x00);	//清屏
   }
}


void LCD12864DisplayPictrue(unsigned char y,unsigned char x,unsigned char px,unsigned char py, unsigned char *pp)	//在任意位置显示任意大小的图片函数
//y-起始行（数值0-63），x-起始列（16位宽，数值0-7），
//px-图片宽度，py-图片高度，pp-指针指向图片数组
//因为上下屏的地址不连续，要在任意位置显示完整的图像，处理起来比较繁琐
{
	unsigned char i,j,k;
	Clear12864Screen();	//清屏
	if(y<32)	//如果起始行在上半屏
	{
		k=32-y;	//算出上半屏的行数
		for(i=0;i<k;i++,y++)	//上半屏行数
		{
			Write12864Command(0x80|y);	//写行地址（垂直地址）
			Write12864Command(0x80|x);	//写列地址（水平地址）
			for(j=0;j<px/8;j++)
				Write12864Data(pp[i*px/8+j]);	//写图片数据
		}
		y=0;	//下半屏起始行，接上半屏继续写数据
		for(;i<py;i++,y++)	//下半屏剩下的行数
		{
			Write12864Command(0x80|y);//写行地址（垂直地址）
			Write12864Command(0x80|(8+x));//写列地址（水平地址）
			for(j=0;j<px/8;j++)
			Write12864Data(pp[i*px/8+j]);//写图片数据
		}
	}
	else //如果起始行在下半屏
	{
		for(i=0;i<py;i++,y++)//行数
		{
			Write12864Command(0x80|(y-32));//写行地址（垂直地址）
			Write12864Command(0x80|(8+x));//写列地址（水平地址）
			for(j=0;j<px/8;j++)
			Write12864Data(pp[i*px/8+j]);//写图片数据
		}
	}
}


/**********************************************/
/* 函数功能:LCD12864 初始化函数               */
/* 入口参数:无                                */
/**********************************************/
void LCD12864_Init()
{
	rst=0;	//复位LCD12864
	Delay_ms(30);
	rst=1;
	Delay_ms(20);
	Write12864Command(0x30);	//功能设定：8位控制方式，使用基本指令
	Write12864Command(0x08);	//显示关
	Write12864Command(0x01);	//清屏
	Write12864Command(0x06);	//地址计数器加一、光标右移
	Write12864Command(0x0c);	//显示开
}
