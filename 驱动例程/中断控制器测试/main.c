
#include <REGX52.H>

void main()
{
    ES = 1;                     //ʹ�ܴ����ж�
    EA = 1;
	RI=0;
	TI=0;
	RI=1;
	while(1)
	{

	}

}
/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
       // RI = 0;                 //���RIλ
        P0 = SBUF;              //P0��ʾ��������
     }
    if (TI)
    {
        TI = 0;                 //���TIλ
    }
}