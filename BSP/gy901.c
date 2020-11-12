#include "data.h"
#include "gy901.h"

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;

u8 second;


void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;
	}
}


void JY901_Read(void)
{
		second=stcTime.ucSecond;
		a_x=(float)stcAcc.a[0]/32768*16*9.8;//float����ǿ�ƣ���Ȼ�õ���a_xΪ0ֵ
		a_y=(float)stcAcc.a[1]/32768*16*9.8;
		a_z=(float)stcAcc.a[2]/32768*16*9.8;
		w_x=(float)stcGyro.w[0]/32768*2000;//(��/s)
		w_y=(float)stcGyro.w[1]/32768*2000;
		w_z=(float)stcGyro.w[2]/32768*2000;
		angle_roll=(float)stcAngle.Angle[0]/32768*180;//(��)
		angle_pitch=(float)stcAngle.Angle[1]/32768*180;
		angle_yaw=(float)stcAngle.Angle[2]/32768*180;
}

