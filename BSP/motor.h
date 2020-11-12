#ifndef __MOTOR_H__
#define __MOTOR_H__



#define abs(x) ((x)>0? (x):(-(x)))



void motor_Set(int16_t motor201,int16_t motor202,int16_t motor203,int16_t motor204);
float Velocity_Control_motor(float current_velocity_motor,float target_velocity_motor);
float Position_Control_motor(float current_position_motor,float target_position_motor);

#endif
