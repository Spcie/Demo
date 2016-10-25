#include "sys.h"
#include "delay.h"
#include "hcsr04.h"
/********************************************************************************************************
超声波测距模块 HC-SR04 驱动
占用资源：TIM2通道1 PA0  
          PA2控制端 Trig   PA0接收端Echo
注意： HC-SR04 必须接5V的Vcc  不可热拔插
2014.8.21
By Spice
********************************************************************************************************/
/********************************************宏定义*****************************************************/
#define    Trig      PAout(2)
/*******************************************变量定义****************************************************/
u8  TIM2CH1_CAPTURE_STA=0;	                              //输入捕获状态		    				
u16	TIM2CH1_CAPTURE_VAL;	                                //输入捕获值

/*******************************************函数声明****************************************************/
int Catch_Init(int arr,int psc);
int GPIOA2_Configuration(void);
/********************************************************************************************************
函数名：HCSR04_Init
描述：HCSR04模块初始化
输入：无
输出：0
********************************************************************************************************/
int HCSR04_Init(void)
{
	GPIOA2_Configuration();
	Catch_Init(0xffff,72-1);
	return 0;
}
/********************************************************************************************************
函数名: Catch_Init
描述：输入捕获初始化
输入：psc 预分频值  arr自动重装值
输出：
********************************************************************************************************/
int Catch_Init(int arr,int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	   //使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;               //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;            //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						              //PA0 下拉
	
		//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr;                   //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	              //预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
		//初始化TIM2输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;                //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;                   //IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);         //允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM2,ENABLE ); 	                                    //使能定时器2
	return 0;
}
/********************************************************************************************************
函数名：GPIOA1_Configuration
描述：PA2配置函数，
输入：无
输出：0
********************************************************************************************************/
int GPIOA2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	return 0;
}
/********************************************************************************************************
函数名：TIM2_IRQHandler
描述：定时器2中断服务函数
输入：无
输出：无
********************************************************************************************************/
void TIM2_IRQHandler(void)
{ 
 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM2CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
	  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM2CH1_CAPTURE_STA=0;			//清空
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);     //把计数器的值修改为0
				TIM2CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
 
}



/********************************************************************************************************
函数名：HCSR04_GetValue
描述：HCSR04完成一次测量并输出距离值
输入：无
输出：Distance 距离值  单位 cm
********************************************************************************************************/
float HCSR04_GetValue(void)
{
	float Distance;
	float temp;
	Trig = 1;
	delay_us(10);
	Trig = 0;
	while(1)                                    //进入循环等待
		{
			if(TIM2CH1_CAPTURE_STA&0X80)            //如果捕捉到一个高电平
			{
				temp=TIM2CH1_CAPTURE_STA&0X3F;
				temp*=65536;				                 	//溢出时间总和
				temp+=TIM2CH1_CAPTURE_VAL;		        //得到总的高电平时间	
				TIM2CH1_CAPTURE_STA=0;		          	//开启下一次捕获
				goto AS;
		  }
	  }
AS:	Distance = temp*1.7/100;
	 return Distance;
}
/************************************************结束***************************************************/




