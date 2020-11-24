// Header:"Uart1.h"
// File Name: Uart1
// Author: cmm
// Date:20200821
// Description:  Uart0Ϊ���ض˿� �ṩ����debug��־
#include "STC8G_USER.H"
//#include "intrins.h"


#include "MultiSystem_UART.h"
char Uart1_wptr;
char Uart1_rptr;
char Uart1_buffer[16];
bit Uart1_busy = 0;
void Uart1Init()
{
    /*Uart1����ģʽ����*/
    SCON = 0x50;
    //    SM2=0;//������ͨ�ſ���λ
    //    SM1=1;//ģʽ1 �ɱ�8λ
    //    SM0=0;
    //    REN=1;//�������
    //    TI=0;//��ձ�־λ
    //    RI=0;
    //����������
    T2L = 0xe6;                                 //65536-11059200/115200/4=0FFE8H
    T2H = 0xff;
    AUXR = 0x15;                                //������ʱ��
    /*�ж�*/
    ES = 1;                                     //ʹ�ܴ����ж�
    EA = 1;
}
void Uart1Send(char dat)
{

    while (Uart1_busy);
    Uart1_busy = 1;
    SBUF = dat;

}
void Uart1SendStr(const char* str)
{

    char strTmp = 0;

    if (str != 0)
    {
        strTmp = *str;
    }
    while (strTmp != 0)
    {
        Uart1Send(strTmp);
        str++;
        strTmp = *str;
    }
}
void UART1_Isr() interrupt 4
{
    if (TI)
    {
        TI = 0;                                 //���жϱ�־
        Uart1_busy = 0;

    }
    if (RI)
    {
        Uart1_buffer[Uart1_wptr++] = SBUF;
        Uart1_wptr &= 0x0f;
        RI = 0;                                 //���жϱ�־
    }
}
