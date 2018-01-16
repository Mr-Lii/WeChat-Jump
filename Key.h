/**********************************************/
/*    |---------|           |-------------|   */
/*    | 1 2 3 A |       \\  | 1  2  3  11 |   */
/*    | 4 5 6 B | =======\\ | 4  5  6  12 |   */
/*    | 7 8 9 C | =======// | 7  8  9  13 |   */
/*    | * 0 # D |       //  |15  0  16  D |   */
/*    |---------|           |-------------|   */
/**********************************************/
#define key P1	//�ӿ�	P1=0x0F(0000 1111):P1^0=1;P1^1=1;P1^2=1;P1^3=1;P1^4=0;P1^5=0;P1^6=0;P1^7=0;

int val;
int ShuWei=0;

void key_scan()
{
	static char a,b;	//a�����ж��ǲ��ǵ�һ�ΰ��£�b������΢��ʱ
	char state;	//��¼��ֵ
	key=0x0F;
	if(key!=0x0F)	//����а�������
	{
		state=key;	//��¼��һ�ΰ���״̬
		if(a==0)	//�����һ�ΰ���
		{
			b++;	//��΢��ʱ������Ҫ��ʱ10ms����ߵ�Ƭ��Ч��
			if(b>3)
			{
				if(key==state)	//�жϼ�ֵ�Ƿ�͵�һ��һ��
				{
					a=1,b=0;	//��λ״ֵ̬ a==1˵���Ѿ����£��ȴ������ɿ�
					key=0x0F;	//������ 0000 1111
					switch(key)
					{
						case(0x0E):	//��һ�У�0000 1110
							val=1;
						break;
						
						case(0x0D):	//�ڶ��У�0000 1101
							val=4;
						break;
						
						case(0x0B):	//�����У�0000 1011
							val=7;
						break;
						
						case(0x07):	//�����У�0000 0111
							val=10;
						break;
					}
					
					key=0xF0;	//������					
					switch(key)
					{
						case(0xE0):	//��һ�У�1110 0000
							val=val;
						
							if(val==10) val=15;	//*
						break;
						
						case(0xD0):	//�ڶ��У�1101 0000
							val=val+1;
							
							if(val==11) val=0;	//��0
						break;
						
						case(0xB0):	//�����У�1011 0000
							val=val+2;
						
						if(val==12) val=16;	//#
						break;
						
						case(0x70):	//�����У�0111 0000
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
		a=b=0;	//�ɿ���λ״̬��
	}
}