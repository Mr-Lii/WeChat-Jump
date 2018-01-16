#include <Delay.h>

sbit E=P3^4;	//����ʹ��
sbit RW=P3^3;	//��дѡ��
sbit RS=P3^2;	//��������ѡ��
sbit rst=P3^5;	//12864��λ

//char code sign[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};



unsigned char code pictrue2[192]={	//ͼƬ������è48*32
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


void Check12864State(void)	//LCD12864���״̬����
{
	P2=0xff;
	E=0;	//��״̬ǰ�������ߵ�״̬
	RS=0;
	RW=1;
	E=1;	//���ߣ���״̬
	while((P2&0x80)==0x80);	//�ȴ�����
	E=0;	//д������������ߵ�״̬
	RS=1;
	RW=0;
}


void Write12864Command( unsigned char com)	//LCD12864д�����
{ 
	Check12864State();	//���״̬
	P2=com;	//��ֵ
	E=0;	//д����ǰ�������ߵ�״̬
	RS=0;
	RW=0;
	E=1;	//���ߣ�д����
	E=0;	//д������������ߵ�״̬
	RS=1;
	RW=1;
}
 

void Write12864Data( unsigned char dat)	//LCD12864д���ݺ���
{
	Check12864State();	//���״̬
	P2=dat;	//��ֵ
	E=0;	//д����ǰ�������ߵ�״̬
	RS=1;
	RW=0;
	E=1;	//���ߣ�д����
	E=0;	//д���ݺ��������ߵ�״̬
	RS=0;
	RW=1;
}
 

void LCD12864DisplayValue( unsigned char y,unsigned char x,int v)	//��ָ����λ����ʾ����ֵ������unsigned char ��Χ��256
//y-����ֵ0-3��x-����ֵ0-7��v-����
{
	unsigned char row;
	Write12864Command(0x30);	//����ָ��
	Write12864Command(0x06);	//��ַ�������Զ����ԣ��������
	
	switch(y)	//�����к�ѡ���е�ַ
	{
		case 0:row=0x80;break;	//��һ���׵�ַ
		case 1:row=0x90;break;	//�ڶ����׵�ַ
		case 2:row=0x88;break;	//�������׵�ַ
		case 3:row=0x98;break;	//�������׵�ַ
		default:;
	}
	
	Write12864Command(row+x);	//д��ַ
	Write12864Data(0x30+v/100);	//��λ���Ƚ����ֻ���ASCII�룬��д��
	Write12864Data(0x30+v%100/10);	//ʮλ/�Ƚ����ֻ���ASCII�룬��д��
	Write12864Data(0x30+v%10);	//��λ���Ƚ����ֻ���ASCII�룬��д��
	
}    


void LCD12864DisplayString( unsigned char y,unsigned char x, unsigned char *pstr)	//��ָ����λ����ʾ�ַ���(���ֺ�ASCII���ַ�)����
//y-����ֵ0-3��x-����ֵ0-7��pstr-�ַ���ָ��
//12864������ʾ32�����֣�����ÿ��8������һ����ַ��Ӧһ������
//������ʾ64��ASCII���ַ�������ÿ��16������һ����ַ��Ӧ�����ַ�
//Ϊ��ʵ���Զ����й��ܣ���������ȽϷ���
{
	unsigned char row,n=0;
	Write12864Command(0x30);	//����ָ��
	Write12864Command(0x06);	//��ַ�������Զ����ԣ��������
	switch(y)	//�����к�ѡ���е�ַ
	{
		case 0:row=0x80;break;	//��һ���׵�ַ
		case 1:row=0x90;break;	//�ڶ����׵�ַ
		case 2:row=0x88;break;	//�������׵�ַ
		case 3:row=0x98;break;	//�������׵�ַ
		default:;
	}
	Write12864Command(row+x);	//д��ַ
	while(*pstr!='\0')
	{
		Write12864Data(*pstr);	//д�ַ�
		pstr++;
		n++;	//����
		if((n+x*2)==16)	//���һ��д�꣬����д�ڶ���
		{
			if(y==0) Write12864Command(0x90);	//д��һ�е�ַ
			else if(y==1) Write12864Command(0x88);	//д��һ�е�ַ
			else if(y==2) Write12864Command(0x98);	//д��һ�е�ַ
			else ;
		}
		else if((n+x*2)==32)	//����ڶ���д�꣬����д������
		{
			if(y==0) Write12864Command(0x88);	//д��һ�е�ַ
			else if(y==1) Write12864Command(0x98);	//д��һ�е�ַ
			else ;
		}
		else if((n+x*2)==48)	//���������д�꣬����д������
		{
			if(y==0) Write12864Command(0x98);	//д��һ�е�ַ
			else ;
		}
		else ;
	}
}


void Clear12864Screen()	//ͼƬģʽ��������
{
	unsigned char i,j;
	Write12864Command(0x34);	//�����趨��8λ���Ʒ�ʽ��ʹ������ָ��
	Write12864Command(0x36);//ʹ������ָ���ͼ��ʾ����
	for(i=0;i<32;i++)
	//ST7920�ɿ���256*32����32��256�У�����12864Һ��ʵ�ʵ��е�ַֻ��0-31��,
	//12864Һ����32-63�е�����0-31�е�ַ�ӵ�128�л���һ�������,���Է�Ϊ������������
	//Ҳ����˵��0�к͵�32��ͬ��һ�У��е�ַ��ͬ;��1�к͵�33��ͬ��һ�У��Դ�����
   {                           
		Write12864Command(0x80|i);	//д�е�ַ����ֱ��ַ��
		Write12864Command(0x80);	//д�е�ַ��ˮƽ��ַ��
		for(j=0;j<32;j++)
		Write12864Data(0x00);	//����
   }
}


void LCD12864DisplayPictrue(unsigned char y,unsigned char x,unsigned char px,unsigned char py, unsigned char *pp)	//������λ����ʾ�����С��ͼƬ����
//y-��ʼ�У���ֵ0-63����x-��ʼ�У�16λ����ֵ0-7����
//px-ͼƬ��ȣ�py-ͼƬ�߶ȣ�pp-ָ��ָ��ͼƬ����
//��Ϊ�������ĵ�ַ��������Ҫ������λ����ʾ������ͼ�񣬴��������ȽϷ���
{
	unsigned char i,j,k;
	Clear12864Screen();	//����
	if(y<32)	//�����ʼ�����ϰ���
	{
		k=32-y;	//����ϰ���������
		for(i=0;i<k;i++,y++)	//�ϰ�������
		{
			Write12864Command(0x80|y);	//д�е�ַ����ֱ��ַ��
			Write12864Command(0x80|x);	//д�е�ַ��ˮƽ��ַ��
			for(j=0;j<px/8;j++)
				Write12864Data(pp[i*px/8+j]);	//дͼƬ����
		}
		y=0;	//�°�����ʼ�У����ϰ�������д����
		for(;i<py;i++,y++)	//�°���ʣ�µ�����
		{
			Write12864Command(0x80|y);//д�е�ַ����ֱ��ַ��
			Write12864Command(0x80|(8+x));//д�е�ַ��ˮƽ��ַ��
			for(j=0;j<px/8;j++)
			Write12864Data(pp[i*px/8+j]);//дͼƬ����
		}
	}
	else //�����ʼ�����°���
	{
		for(i=0;i<py;i++,y++)//����
		{
			Write12864Command(0x80|(y-32));//д�е�ַ����ֱ��ַ��
			Write12864Command(0x80|(8+x));//д�е�ַ��ˮƽ��ַ��
			for(j=0;j<px/8;j++)
			Write12864Data(pp[i*px/8+j]);//дͼƬ����
		}
	}
}


/**********************************************/
/* ��������:LCD12864 ��ʼ������               */
/* ��ڲ���:��                                */
/**********************************************/
void LCD12864_Init()
{
	rst=0;	//��λLCD12864
	Delay_ms(30);
	rst=1;
	Delay_ms(20);
	Write12864Command(0x30);	//�����趨��8λ���Ʒ�ʽ��ʹ�û���ָ��
	Write12864Command(0x08);	//��ʾ��
	Write12864Command(0x01);	//����
	Write12864Command(0x06);	//��ַ��������һ���������
	Write12864Command(0x0c);	//��ʾ��
}
