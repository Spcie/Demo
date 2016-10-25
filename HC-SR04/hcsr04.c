#include "sys.h"
#include "delay.h"
#include "hcsr04.h"
/********************************************************************************************************
���������ģ�� HC-SR04 ����
ռ����Դ��TIM2ͨ��1 PA0  
          PA2���ƶ� Trig   PA0���ն�Echo
ע�⣺ HC-SR04 �����5V��Vcc  �����Ȱβ�
2014.8.21
By Spice
********************************************************************************************************/
/********************************************�궨��*****************************************************/
#define    Trig      PAout(2)
/*******************************************��������****************************************************/
u8  TIM2CH1_CAPTURE_STA=0;	                              //���벶��״̬		    				
u16	TIM2CH1_CAPTURE_VAL;	                                //���벶��ֵ

/*******************************************��������****************************************************/
int Catch_Init(int arr,int psc);
int GPIOA2_Configuration(void);
/********************************************************************************************************
��������HCSR04_Init
������HCSR04ģ���ʼ��
���룺��
�����0
********************************************************************************************************/
int HCSR04_Init(void)
{
	GPIOA2_Configuration();
	Catch_Init(0xffff,72-1);
	return 0;
}
/********************************************************************************************************
������: Catch_Init
���������벶���ʼ��
���룺psc Ԥ��Ƶֵ  arr�Զ���װֵ
�����
********************************************************************************************************/
int Catch_Init(int arr,int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	   //ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;               //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;            //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						              //PA0 ����
	
		//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr;                   //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	              //Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
		//��ʼ��TIM2���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;                //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;                   //IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                          //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);         //��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM2,ENABLE ); 	                                    //ʹ�ܶ�ʱ��2
	return 0;
}
/********************************************************************************************************
��������GPIOA1_Configuration
������PA2���ú�����
���룺��
�����0
********************************************************************************************************/
int GPIOA2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	return 0;
}
/********************************************************************************************************
��������TIM2_IRQHandler
��������ʱ��2�жϷ�����
���룺��
�������
********************************************************************************************************/
void TIM2_IRQHandler(void)
{ 
 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM2CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
	  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM2CH1_CAPTURE_STA=0;			//���
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);     //�Ѽ�������ֵ�޸�Ϊ0
				TIM2CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
 
}



/********************************************************************************************************
��������HCSR04_GetValue
������HCSR04���һ�β������������ֵ
���룺��
�����Distance ����ֵ  ��λ cm
********************************************************************************************************/
float HCSR04_GetValue(void)
{
	float Distance;
	float temp;
	Trig = 1;
	delay_us(10);
	Trig = 0;
	while(1)                                    //����ѭ���ȴ�
		{
			if(TIM2CH1_CAPTURE_STA&0X80)            //�����׽��һ���ߵ�ƽ
			{
				temp=TIM2CH1_CAPTURE_STA&0X3F;
				temp*=65536;				                 	//���ʱ���ܺ�
				temp+=TIM2CH1_CAPTURE_VAL;		        //�õ��ܵĸߵ�ƽʱ��	
				TIM2CH1_CAPTURE_STA=0;		          	//������һ�β���
				goto AS;
		  }
	  }
AS:	Distance = temp*1.7/100;
	 return Distance;
}
/************************************************����***************************************************/




