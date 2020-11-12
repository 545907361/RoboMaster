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
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。
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
		a_x=(float)stcAcc.a[0]/32768*16*9.8;//float必须强制，不然得到的a_x为0值
		a_y=(float)stcAcc.a[1]/32768*16*9.8;
		a_z=(float)stcAcc.a[2]/32768*16*9.8;
		w_x=(float)stcGyro.w[0]/32768*2000;//(°/s)
		w_y=(float)stcGyro.w[1]/32768*2000;
		w_z=(float)stcGyro.w[2]/32768*2000;
		angle_roll=(float)stcAngle.Angle[0]/32768*180;//(°)
		angle_pitch=(float)stcAngle.Angle[1]/32768*180;
		angle_yaw=(float)stcAngle.Angle[2]/32768*180;
}

