#include "main.h"
/*-----USART2_RX-----PD6----*/ 
/*-----USART2_TX-----PD5----*/
//for D-BUS
  //  temp sbus decode channel data
float a_x,a_y,a_z;      //���ٶ�
float w_x,w_y,w_z;      //���ٶ�
float angle_pitch,angle_roll,angle_yaw;//�Ƕ�
u8 mpu_flag = 0;
unsigned char Temp[33];
void USART2_Configuration(void)
{
    USART_InitTypeDef usart2;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	  GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 

    gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD,&gpio);

    usart2.USART_BaudRate = 115200;
    usart2.USART_WordLength = USART_WordLength_8b;
    usart2.USART_StopBits = USART_StopBits_1;
    usart2.USART_Parity = USART_Parity_No;
    usart2.USART_Mode = USART_Mode_Rx;
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2,&usart2);

    USART_Cmd(USART2,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    
    nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel= DMA_Channel_4;
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    dma.DMA_Memory0BaseAddr = (uint32_t)Temp;
    
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_BufferSize = 33;
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
    DMA_Init(DMA1_Stream5,&dma);

    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Stream5,ENABLE);
		
		delay_ms(10);
}

void DMA1_Stream5_IRQHandler(void)
{
   if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
    {
        DMA_ClearFlag(DMA1_Stream5, DMA_IT_TCIF5);
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
				delay_ms(2);
				//printf("pitch==%.1f---roll==%.1f---yaw==%.1f---w_y==%f\r\n",angle_pitch,angle_roll,angle_yaw,w_y );
        if(Temp[0]==0x55)//֡ͷ  
        {
            mpu_flag=1;
            /*if(Temp[1]==0x51)
            {
                a_x = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X����ٶ�
                a_y = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y����ٶ�
                a_z = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z����ٶ�
            }*/
            if(Temp[12]==0x52)
            {
                w_x = ((short)(Temp[14]<<8| Temp[13]))/32768.0*2000;      //X����ٶ�
                w_y = ((short)(Temp[16]<<8| Temp[15]))/32768.0*2000;      //Y����ٶ�
               // w_z = ((short)(Temp[18]<<8| Temp[17]))/32768.0*2000;      //Z����ٶ�
            }
            if(Temp[23]==0x53)
            {
                angle_roll = ((short)(Temp[25]<<8| Temp[24]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
                angle_pitch = ((short)(Temp[27]<<8| Temp[26]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
                angle_yaw = ((short)(Temp[29]<<8| Temp[28]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
            }
        }
        else
        {
            mpu_flag=0;
            USART2_Configuration();//��ⲻ������ ���ô���
        }
    }
}
