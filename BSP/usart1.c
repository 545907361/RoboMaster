#include "usart1.h"
/*-----USART1_TX-----PA9-----*/
/*-----USART1_RX-----PA10----*/
//使用usart1 DMA通道读取MPU6050数据

u8   Key_w,Key_s,Key_a,Key_d,Key_shift,Key_ctrl,Key_q,Key_e;
s16  Mouse_value_l,Mouse_value_r,Mouse_value_x,Mouse_value_y;
s16  Push_Left_UAD,Push_Left_LAR,Push_Right_UAD,Push_Right_LAR;
u8	 Switch_Left,Switch_Right;
u8 remote_flag=0;
/*****************************  自稳切换 *************************************/
float target_sign = 0;   //记录上一次遥控器的数据
u8 count_num = 0;        //遥控器数值不变的累加
u8 sign = 1;          //自稳状态切换标志 1自稳 0遥控器控制
float target_bias = -2.1;  //遥控转自稳的偏置

unsigned char sbus_rx_buffer[18];
int  sbus_channel_temp[17];
void USART1_Configuration(void)
{
    USART_InitTypeDef usart1;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7 ,GPIO_AF_USART1); 
	
    gpio.GPIO_Pin = GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&gpio);

    usart1.USART_BaudRate =100000;
    usart1.USART_WordLength = USART_WordLength_8b;
    usart1.USART_StopBits = USART_StopBits_1;
    usart1.USART_Parity = USART_Parity_No;
    usart1.USART_Mode = USART_Mode_Rx;
    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&usart1);

    USART_Cmd(USART1,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    
    nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_DeInit(DMA2_Stream2);
    dma.DMA_Channel= DMA_Channel_4;
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
    
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_BufferSize = 18;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_Mode_Normal;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2,&dma);

    DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA2_Stream2,ENABLE);
		
		delay_ms(10);
}

void DMA2_Stream2_IRQHandler(void)
{
   if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
    {
        DMA_ClearFlag(DMA2_Stream2, DMA_IT_TCIF2);
        DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);

				delay_ms(1);
      remote_flag=1;  
			
		  sbus_channel_temp[0] = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;//channel 0
      sbus_channel_temp[1] = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;//channel 1
      sbus_channel_temp[2] = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;//channel 2
      sbus_channel_temp[3] = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;//channel 3
      sbus_channel_temp[4] = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;//radio_switch_left
      sbus_channel_temp[5] = ((sbus_rx_buffer[5] >> 4)& 0x0003);//radio_switch_right
       
      sbus_channel_temp[6] = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8);//Mouse X
      sbus_channel_temp[7] = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8);//Mouse Y
      sbus_channel_temp[8] = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8);//Mouse Z
      sbus_channel_temp[9] = sbus_rx_buffer[12];//Mouse left
      sbus_channel_temp[10] = sbus_rx_buffer[13];//Mouse right
      sbus_channel_temp[11] = sbus_rx_buffer[14]| (sbus_rx_buffer[15] << 8);//Key
      sbus_channel_temp[12] = sbus_rx_buffer[16]| (sbus_rx_buffer[17] << 8);//NULL  
    }
}
//取出遥控值
void Radio_Judge(void)
{
//推杆值范围：-660~660
//拨键值范围：上1，中3，下2
		Push_Left_UAD=sbus_channel_temp[3]-1024;
		Push_Left_LAR=sbus_channel_temp[2]-1024;
		Push_Right_UAD=sbus_channel_temp[1]-1024;
		Push_Right_LAR=sbus_channel_temp[0]-1024;
		Switch_Left=((sbus_rx_buffer[5] >> 4)& 0x000C)>>2;
		Switch_Right=(sbus_rx_buffer[5] >> 4)& 0x0003;
	
//取出鼠标值
//鼠标按键：按下1，弹起0
//鼠标滚动：向右或向上>60000
	  Mouse_value_l = sbus_channel_temp[9];
    Mouse_value_r = sbus_channel_temp[10];
    Mouse_value_x = sbus_channel_temp[6];
    Mouse_value_y = sbus_channel_temp[7];
	
//取出键盘值
//按键值范围：按下1，弹起0
		Key_w= sbus_channel_temp[11]&0x01;
	  Key_s= (sbus_channel_temp[11]>>1)&0x01;
	  Key_a= (sbus_channel_temp[11]>>2)&0x01;
	  Key_d= (sbus_channel_temp[11]>>3)&0x01;
  	Key_shift=(sbus_channel_temp[11]>>4)&0x01;
  	Key_ctrl=(sbus_channel_temp[11]>>5)&0x01;
  	Key_q= (sbus_channel_temp[11]>>6)&0x01;
	  Key_e= (sbus_channel_temp[11]>>7)&0x01;
}
