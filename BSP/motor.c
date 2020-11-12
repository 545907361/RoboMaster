#include  "main.h"




//���ٵ��
//�����������Ӧ����ŵ�������Χ-32768~32768
void motor_Set(int16_t motor201,int16_t motor202,int16_t motor203,int16_t motor204)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x200;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(motor201 >> 8);
    tx_message.Data[1] = (unsigned char)motor201;
    tx_message.Data[2] = (unsigned char)(motor202 >> 8);
    tx_message.Data[3] = (unsigned char)motor202;
    tx_message.Data[4] = (unsigned char)(motor203 >> 8);
    tx_message.Data[5] = (unsigned char)motor203;
    tx_message.Data[6] = (unsigned char)(motor204 >> 8);
    tx_message.Data[7] = (unsigned char)motor204;
    
    CAN_Transmit(CAN2,&tx_message);
}


/********************************************************************************
                         820R�������ٶȻ�����
                    ���� 820R��ǰ�ٶ� 820RĿ���ٶ�
*********************************************************************************/
float Velocity_Control_motor(float current_velocity_motor,float target_velocity_motor)
{
    const float v_p = 1.0;
	  const float v_i = 0;
    const float v_d = 10.0001;
    
    static float error_v[2] = {0.0,0.0};
    static float output = 0;
    static float inte = 0; 
    
    if(abs(current_velocity_motor) < GAP)
    {
        current_velocity_motor = 0.0;
    }
    
    error_v[0] = error_v[1];
    error_v[1] = target_velocity_motor - current_velocity_motor;
	  inte += error_v[1]; 


  	if(inte > MAX_INTE)
	  inte = MAX_INTE;
	  if(inte < -MAX_INTE)
	  inte = -MAX_INTE;
    
    output = error_v[1] * v_p 
             + inte * v_i
		         + (error_v[1] - error_v[0]) * v_d;
     
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return output;//cyq:for6015 ����
}

/********************************************************************************
                         820R������λ�û����� 
                    ���� 820R��ǰλ�� 820RĿ��λ��
*********************************************************************************/
float Position_Control_motor(float current_position_motor,float target_position_motor)
{
    
    const float l_p = 1.2;
    const float l_i = 0;
    const float l_d = 0.0001;

    static float error_l[2] = {0.0,0.0};
    static float output = 0;
    static float inte = 0; 
    
    error_l[0] = error_l[1];
    error_l[1] = target_position_motor - current_position_motor;
    inte += error_l[1]; 


  	if(inte > MAX_INTE)
	  inte = MAX_INTE;
	  if(inte < -MAX_INTE)
	  inte = -MAX_INTE;

    output = error_l[1] * l_p 
            + inte * l_i 
            + (error_l[1] - error_l[0]) * l_d;
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    		
    return -output;
}

