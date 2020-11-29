
#include "ModBusSlave.h"
#include "Uart3_STC8G.H"
//����ջ�������Ӧ
#define BUFF_LEN 0x20
typedef enum enumFunCode
{
  //������
  Enum_BroadCast = 0x00,
  //bit
  Enum_ROStat = 0x01,/*�������״̬*/
  Enum_RIDis = 0x02,/*��������ɢ��*/
  Enum_WSOut = 0x05,/*д���������*/
  Enum_WMOut = 0x0F,/*д��������*/
  //byte
  Enum_RIReg = 0x04,/*������Ĵ���*/
  Enum_ROReg = 0x03,/*������Ĵ���*/
  Enum_WSReg = 0x06,/*д�����Ĵ���*/
  Enum_WMReg = 0x10,/*д����Ĵ���*/
  //Enum_RWReg = 0x17,/*�� / д�Ĵ���*/
  Enum_MWReg = 0x16,/*����д�Ĵ���*/
  //Other
  Enum_GCECout = 0x0B,/*��ȡͨ���¼�����*/
} FunCode;


typedef struct _MODBUS_ROREG
{
  char iDevID;
  char iCmd;
  uint16_t iAddr;
  uint16_t iNum;
  uint8_t  CrcL;
  uint8_t  CrcH;
} *LpRoReg;
typedef struct _MODBUS_ROREG_RES
{
  char iDevID;
  char iCmd;
  uint8_t iByteNum;
  uint16_t RegData[(BUFF_LEN-3)/2];
//  uint8_t  CrcL;
//  uint8_t  CrcH;
} *LpRoRegRes;

char ReadHoldRegs(LpRoReg lpData);
void ModBusAnalysis(CModBus* lpmodbus)
{
  char iRec;
  switch(lpmodbus->iCmd[1])
  {
  case Enum_ROReg://��һ���������ּĴ�����ȡ�õ�ǰ�Ķ�����ֵ
    iRec=ReadHoldRegs((LpRoReg)lpmodbus->iCmd);
    break;
  case Enum_WSReg:
    break;
  }
}
	//RTUģʽ
	//����ַ���������롿����������������1��������������n����CRC���ֽڡ���CRC���ֽڡ�
	uint16_t RTU_CalcCrc(const uint8_t *chData,uint8_t uNo)/*���ݳ���*/
	{
	  uint16_t crc = 0xffff;
	  uint16_t i, j;
	  for (i = 0; i < uNo; i++)
	  {
		crc ^= chData[i];
		for (j = 0; j < 8; j++)
		{
		  if (crc & 1)
		  {
			crc >>= 1;
			crc ^= 0xA001;
		  }
		  else
			crc >>= 1;
		}
	  }
	  return (crc);
	}
//����	U39	0x03,0x06	0x0000
//����	U39	0x03,0x06	0x0001
//����	U39	0x03,0x06	0x0007
//10ѡ1	U39	0x03,0x06	0x000D

char ReadHoldRegs(LpRoReg lpData)
{//���ֽ����ݶ�ȡ ���Դ���  ����ͨ��
  char tab[BUFF_LEN],ilen;
  LpRoRegRes response=tab;
  char irec=0,i;
  uint16_t iCurAddr;
  char* lpTmp8u;
  response->iDevID=lpData->iDevID;
  response->iCmd=lpData->iCmd;
  response->iByteNum=0;
  for(i=0; i<lpData->iNum; i++)
  {
    iCurAddr=lpData->iAddr+i;
    if(iCurAddr==0x0000)
    {
     response->RegData[i]=i;
    }
    else if(iCurAddr<0x0007)
    {
    response->RegData[i]=i;
    }
    else if(iCurAddr<0x000D)
    {
     response->RegData[i]=i;
    }
    else if(iCurAddr==0x000D)
    {
     response->RegData[i]=i;
    }
    else
    {
      irec=0x04;
    }
  }
  response->iByteNum=i;//װ�����ݳ���
  //response->RegData[i]=0;
  ilen=(char*)(&response->RegData[i])-(char*)response;//�����������ݳ���
  iCurAddr=RTU_CalcCrc((char*)response,ilen);//����У��
  lpTmp8u=(char*)(&response->RegData[i]);//�õ�װ��crc�ĵ�ַ
  lpTmp8u[0]=iCurAddr&0xff;
  lpTmp8u[1]=(iCurAddr>>8)&0xff;
  ilen=ilen+2;//�����ܳ���
  Uart3SendArray((char*)response,ilen);
  Uart3Send(ilen);
  return irec;
}