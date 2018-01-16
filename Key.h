/**********************************************/
/*    |---------|           |-------------|   */
/*    | 1 2 3 A |       \\  | 1  2  3  11 |   */
/*    | 4 5 6 B | =======\\ | 4  5  6  12 |   */
/*    | 7 8 9 C | =======// | 7  8  9  13 |   */
/*    | * 0 # D |       //  |15  0  16  D |   */
/*    |---------|           |-------------|   */
/**********************************************/
#define key P1	//接口	P1=0x0F(0000 1111):P1^0=1;P1^1=1;P1^2=1;P1^3=1;P1^4=0;P1^5=0;P1^6=0;P1^7=0;

int val;
int ShuWei=0;

void key_scan()
{
	static char a,b;	//a用于判断是不是第一次按下，b用于稍微延时
	char state;	//记录键值
	key=0x0F;
	if(key!=0x0F)	//如果有按键按下
	{
		state=key;	//记录第一次按键状态
		if(a==0)	//如果第一次按下
		{
			b++;	//略微延时，不需要延时10ms以提高单片机效率
			if(b>3)
			{
				if(key==state)	//判断键值是否和第一次一致
				{
					a=1,b=0;	//复位状态值 a==1说明已经按下，等待后续松开
					key=0x0F;	//测试行 0000 1111
					switch(key)
					{
						case(0x0E):	//第一行：0000 1110
							val=1;
						break;
						
						case(0x0D):	//第二行：0000 1101
							val=4;
						break;
						
						case(0x0B):	//第三行：0000 1011
							val=7;
						break;
						
						case(0x07):	//第四行：0000 0111
							val=10;
						break;
					}
					
					key=0xF0;	//测试列					
					switch(key)
					{
						case(0xE0):	//第一列：1110 0000
							val=val;
						
							if(val==10) val=15;	//*
						break;
						
						case(0xD0):	//第二列：1101 0000
							val=val+1;
							
							if(val==11) val=0;	//改0
						break;
						
						case(0xB0):	//第三列：1011 0000
							val=val+2;
						
						if(val==12) val=16;	//#
						break;
						
						case(0x70):	//第四列：0111 0000
							val=val+13;
							
							if(val==14) val=11;	//A
							else if(val==17) val=12;	//B
							else if(val==20) val=13;	//C
							else if(val==23) val=14;	//D
						break;						
					}
					
					if(val<=9) ShuWei++;
					else if(val==15)
					{
						ShuWei=0;
						val=0;
					}
				}
			}
		}
	}
	else
	{
		a=b=0;	//松开后复位状态机
	}
}
