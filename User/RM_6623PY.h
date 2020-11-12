#ifndef _RM6623PY_H_
#define _RM6623PY_H_
#include "stm32f4xx.h"

//Pitch轴
#define CNOMTROL_V 40  //电机运动速度
#define CNOMTROL_MAX 5300  //电机运动最大角度   //2850
#define CNOMTROL_MIN 4500  //电机运动最小角度   //2400
extern u8 first;          //云台开关，1开启，0关闭
extern u8 first_times;		 //云台稳定累计值，当达上限，打开云台开关
extern u8 error_gy901;   //gy901是否出现问题
/********************************************/
extern float target;
extern float Pitch_speed;
/********************************************/
extern float GAP;
extern float ESC_MAX;
extern float INTE_MAX;
extern float RM6623_pitch_ap,RM6623_pitch_ai,RM6623_pitch_vp,RM6623_pitch_vi,RM6623_pitch_vd,RM6623_pitch_x1,RM6623_pitch_x2;
extern float RM6623_yaw_ap,RM6623_yaw_ai,RM6623_yaw_vp,RM6623_yaw_vi,RM6623_yaw_vd,RM6623_yaw_x1,RM6623_yaw_x2;
extern float gy_ap,gy_ai,gy_vp,gy_vi,gy_vd,gy_x1,gy_x2;
void  APP_Pitch_PID(void);
void APP_Yaw_PID(void);
void Pitch_PID(void);
extern s16 pitch_current_angle,pitch_target_angle,pitch_target_velocity,pitch_current;
extern s16 yaw_current_angle,  yaw_target_angle,  yaw_target_velocity,  yaw_current;
extern s16 pitch_current_velocity;
extern s16 yaw_current_velocity;

float Velocity_Control_Pitch(float current_velocity_pitch,float target_velocity_pitch);
float Position_Control_Pitch(float current_position_pitch,float target_position_pitch);
float Velocity_Control_Yaw(float current_velocity_yaw,float target_velocity_yaw);
float Position_Control_Yaw(float current_position_yaw,float target_position_yaw);
float Attitude_RatePID(float Current_rate,float Goal_rate);
float Angle_PID(float Current_angle,float Goal_angle);

#endif
