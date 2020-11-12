#include "main.h"

void Cmd_ESC(int16_t current_pitch,int16_t current_yaw)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x1ff;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(current_yaw >> 8);
    tx_message.Data[1] = (unsigned char)current_yaw;
    tx_message.Data[2] = (unsigned char)(current_pitch >> 8);
    tx_message.Data[3] = (unsigned char)current_pitch;
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    
    CAN_Transmit(CAN1,&tx_message);
}

//底盘电机控制
//输入参数：对应电机号电流，范围-5000~5000
void RM3510_Current_Set(int16_t current_motor1,int16_t current_motor2,int16_t current_motor3,int16_t current_motor4)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x200;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(current_motor1 >> 8);
    tx_message.Data[1] = (unsigned char)current_motor1;
    tx_message.Data[2] = (unsigned char)(current_motor2 >> 8);
    tx_message.Data[3] = (unsigned char)current_motor2;
    tx_message.Data[4] = (unsigned char)(current_motor3 >> 8);
    tx_message.Data[5] = (unsigned char)current_motor3;
    tx_message.Data[6] = (unsigned char)(current_motor4 >> 8);
    tx_message.Data[7] = (unsigned char)current_motor4;
    
    CAN_Transmit(CAN2,&tx_message);
}


void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);      
     if(rx_message.StdId == 0x206)      //Pitch
        {    
						pitch_current_angle=(rx_message.Data[0]*256+rx_message.Data[1]);
					if((angle_pitch-40<target)&&(target<angle_pitch+40))
					{
						if(Push_Right_UAD <= -700 || Push_Right_UAD >= 700) Push_Right_UAD = 0;
						if(error_gy901)
						{
							APP_Pitch_PID();
							target = angle_pitch;
						}
						else
						{
							Pitch_PID();
							pitch_target_angle = pitch_current_angle;
						}

					}
				 }	
        if(rx_message.StdId == 0x205)     //Yaw
        { 
						yaw_current_angle = rx_message.Data[0]*256+rx_message.Data[1];
						APP_Yaw_PID();
        }
				Cmd_ESC(pitch_current,yaw_current);
    }
}
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
    {
       CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
       CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
			//------------------------------------------------------------------------------------------	              
			  if(rx_message.StdId ==0x201)//底盘电机01
				 {   	
					
					 motor201_current_velocity=rx_message.Data[2]*256+rx_message.Data[3];
					 motor201_target_velocity = a-b-c;
					 RM3510_PID1();

				 }	
				 if(rx_message.StdId ==0x202)//底盘电机01
				 {   	
					
					 motor202_current_velocity=rx_message.Data[2]*256+rx_message.Data[3];
					 motor202_target_velocity = -a-b-c;
					 RM3510_PID2();

				 }	
				 if(rx_message.StdId ==0x203)//底盘电机01
				 {   	
					
					 motor203_current_velocity=rx_message.Data[2]*256+rx_message.Data[3];
					 motor203_target_velocity = -(a-b+c);
					 
					RM3510_PID3();

				 }	
				 if(rx_message.StdId ==0x204)//底盘电机01
				 {   	
					
					 motor204_current_velocity=rx_message.Data[2]*256+rx_message.Data[3];
					 motor204_target_velocity = a+b-c;
			   RM3510_PID4();

				 }			 		 			 
//------------------------------------------------------------------------------------------	 
			
   			
	RM3510_Current_Set(motor201_current_velocity,motor202_current_velocity,motor203_current_velocity,motor204_current_velocity);	
		
              
		}
}

