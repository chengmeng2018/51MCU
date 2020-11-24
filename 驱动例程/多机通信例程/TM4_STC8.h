#ifndef __TM4_8_H_
#define __TM4_8_H_

typedef struct
{//����ʱ18H
    int MS;//rang[0,1000]
    int S; //rang[0,0x8000]
} ClockTypedef;
extern ClockTypedef Clock_Tm4;
typedef enum _DelayState {Tm4_Start,Tm4_Stop,Tm4_Wait,Tm4_Ok} DelayState;
//��ʱ����ʱ����
typedef struct
{
    unsigned int DelayMs;//�趨��ʱʱ��
    DelayState 		State;
    unsigned int StartMs;//�ڲ�ʵ�� ��������ʱ��
    unsigned int EndMs;  //�ڲ�ʵ�� ��ʱ����ʱ��
} DelayTypedef;


void TM4_Init();
void GetCurrentTime(ClockTypedef* time);
int SubTime(ClockTypedef* time);
int SubTimeTi(ClockTypedef* time);
//��������ʱ����
void DelayNonBlocking(DelayTypedef* DelayStruct);
//������ʱ����
void DelayBlocking(unsigned int Time);
#endif