#ifndef __RM3510_H__
#define __RM3510_H__
#include <stm32f4xx.h>

#define abs(x) ((x)>0? (x):(-(x)))
#define tyre_ESC_MAX 6000.0
#define GAP 0.0

extern float RM3510_ep,RM3510_ei,RM3510_ed,RM3510_vp,RM3510_vi,RM3510_vd,RM3510_x1,RM3510_x2;

extern s16 motor201_current_electric,motor201_target_electric,motor201_current_velocity,motor201_target_velocity,motor201_current;
extern s16 motor202_current_electric,motor202_target_electric,motor202_current_velocity,motor202_target_velocity,motor202_current;
extern s16 motor203_current_electric,motor203_target_electric,motor203_current_velocity,motor203_target_velocity,motor203_current;
extern s16 motor204_current_electric,motor204_target_electric,motor204_current_velocity,motor204_target_velocity,motor204_current;

float RM3510_Velocity_Control_motor1(float current_velocity_motor,float target_velocity_motor);
float RM3510_Velocity_Control_motor2(float current_velocity_motor,float target_velocity_motor);
float RM3510_Velocity_Control_motor3(float current_velocity_motor,float target_velocity_motor);
float RM3510_Velocity_Control_motor4(float current_velocity_motor,float target_velocity_motor);

float RM3510_Electric_Control_motor1(float current_position_motor,float target_position_motor);
float RM3510_Electric_Control_motor2(float current_position_motor,float target_position_motor);
float RM3510_Electric_Control_motor3(float current_position_motor,float target_position_motor);
float RM3510_Electric_Control_motor4(float current_position_motor,float target_position_motor);

void RM3510_PID1(void);
void RM3510_PID2(void);
void RM3510_PID3(void);
void RM3510_PID4(void);




#endif
