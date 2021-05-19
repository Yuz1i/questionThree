#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "math.h"
#include "ad9958.h"
#include "exti.h"

#define C        2//低通滤波放大倍数
#define ANGLE    57.2957795130//转换为角度值  180 / +3.1415
#define ADC_STEP 1//不需要了，直接用数字量进行运算
#define MOSTVOL   4//最高的电压是4v方便后面进行画图


void draw_background();
u32 SINGLE_FRE = 20000000;
u8 modeflag = 0;//0是扫频，1是点屛
u32 SOURCE_AMP =1.00; //1v ad9958的幅度

//PF0~4 AD9958的控制
//ADC开启通道ADC1_0 pA0 ADC2_1 PA1
//启用dma通道传输数据

//将幅值数字量表示   


void get_pha_amp(u16 adc_data1,u16 adc_data2,u16 * transdata)//对两个ADC的值进行运算需要知道正交源 的幅度以及被测网络的放大倍数
{
	double vol;
	double  pha;

	vol = 2 * sqrt( (adc_data1 * adc_data1 + adc_data2 * adc_data2)) / (SOURCE_AMP * C);
	pha = atan(adc_data2 /adc_data1) * ANGLE;//求得b
	vol = 20 * log(vol / SOURCE_AMP) / log(10);
	*transdata = vol;
	transdata ++;
	*transdata = pha; 
	//printf("vol :%lf ,角度： %lf\r\n",vol,amp);
}

void sweeperfre(void)
{
	while(1)
	{
		//int ampdata[2];
		u16 draw[2];
		u16 amp[2];
		int fre;
		int i = 0;
	
		//先画界面,还得清屏
		LCD_Clear(WHITE);
		draw_background();
	

		//扫频画线
		for(fre = 100000,i = 0 ; fre <= 40000000;fre += 100000)
		{

			//ad9958控制产生两路波
			ad9958_out(2,fre,0,840);
		
			//显示当前的频率
		
		
			//adc 采集
			amp[0] = get_adc1(5);
			amp[1] = get_adc2(5);
			get_pha_amp(amp[0],amp[1],amp);//对采集到的点进行运算
			printf("amp:%d,%d\r\n",amp[0],amp[1]);
			//开始画
			if(i == 0)
			{
				draw[0] = 0;
				draw[1] = 0;
			}
			POINT_COLOR=BLUE;//设置字体为红色 
			LCD_DrawLine(i + 50,700 - draw[0] / 20 * 40,i + 51,700 - amp[0] / 20 * 40);//坐标还未处理需要对坐标进行处理
			printf("draw:%d,%d\r\n",draw[0],draw[1]);
			
			POINT_COLOR=YELLOW;//设置字体为红色 
			LCD_DrawLine(i + 50,700 - ((int)draw[1] % 90) / 90 * 400,i + 51,700 - ((int)amp[1] % 90) / 90 * 400);
			POINT_COLOR=RED;//设置字体为红色
			draw[0] = amp[0];
			draw[1] = amp[1];
			i++;
		}

	}
	
	
	
}

void draw_background()
{
	u8 cir = 0;
	for(cir = 0;cir < 11;cir ++)
	{
		POINT_COLOR=RED;//设置字体为红色
		LCD_DrawLine(50,700 - cir * 40,450,700 - cir * 40);
		LCD_DrawLine(50 + 40 * cir,700,50 + 40 * cir,300);
		
	}
	cir = 0;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"0");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"4M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"8M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"12M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"16M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"20M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"24M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"28M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"32M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"36M");cir ++;
	LCD_ShowString(42 + 40 * cir,720,24,16,16,"40M");cir ++;
	
	cir = 1;
	LCD_ShowString(0,720- 40 * cir,32,16,16,"0.4v");cir ++;
}

void draw_backgroud1(void)//搁置，在屏幕上不好控制
{
	LCD_ShowString(10,50,24 * 3,24,24,"+1");
	LCD_ShowString(10,110,24 * 3,24,24,"+10");
	LCD_ShowString(10,170,24 * 3,24,24,"+100");
	LCD_ShowString(10,230,24 * 3,24,24,"+1k");
	LCD_ShowString(10,290,24 * 3,24,24,"+10k");
	LCD_ShowString(10,350,24 * 3,24,24,"+100k");
	LCD_ShowString(10,410,24 * 3,24,24,"+1M");
	LCD_ShowString(10,470,24 * 3,24,24,"+10M");

	LCD_ShowString(350,50,24 * 3,24,24,"-1");
	LCD_ShowString(350,110,24 * 3,24,24,"-10");
	LCD_ShowString(350,170,24 * 3,24,24,"-100");
	LCD_ShowString(350,230,24 * 3,24,24,"-1k");
	LCD_ShowString(350,290,24 * 3,24,24,"-10k");
	LCD_ShowString(350,350,24 * 3,24,24,"-100k");
	LCD_ShowString(350,410,24 * 3,24,24,"-1M");
	LCD_ShowString(350,470,24 * 3,24,24,"-10M");





}



void single_fre(void)
{
	u16 data[2];//先分别接收两个数据运算后存在这里省空间
	u16 fre;

{
	while(1)
	{
		int past[2];
		int current[2];
		int x,y;
		u8 flag = 1;//第一次触屏
		tp_dev.scan(0);
		
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
						x = lastpos[t][0];
						y = lastpos[t][1];
                        if(flag)
                        {
							current[0] = 0;
                        	current[1] = 0;
							past[0] = tp_dev.x[0];
                        	past[1] = tp_dev.y[0];
                            flag = 0;
                        }
						else
						{
							current[0] = tp_dev.x[0];
                        	current[1] = tp_dev.y[0];
						}
                       
					}
					if((current[0] - past[0] > 10)  | (current[0] - past[0] < -10) | (current[1] - past[1] > 10) | (current[1] - past[1] < -10))
					{
						current[1] = past[1];
						current[0] = past[0];
						if((y < 500 )//加减区
						{
							if(x < 180)
							{
								if(y > 470)
								{
									

								}

							}
							else 
							{


							}



						}
						
				    }
					
					
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
	}



}
	//清屏
	LCD_Clear(WHITE);
	
	//画另一个界面 控制界面
	draw_backgroud1();
	
	
	//ad9958产生波
	ad9958_out(2,SINGLE_FRE,0,1023);
	
	
	
	
	
	//采集信息
	data[0] = get_adc1(5);
	data[1] = get_adc2(5);
	//进行运算
	get_pha_amp(data[0],data[1],data);
	
	
	//显示结果
	LCD_ShowString(4,700,48,24,24,"AMP:");
	LCD_ShowxNum(60,700,data[0],5,24,0);
	
	
	LCD_ShowString(4,730,48,24,24,"PHA:");
	LCD_ShowxNum(60,730,data[1],5,24,0);


	
}
  


 int main(void)
 {	 
	u16 adcx1;
	u16 adcx2;
	float temp;
	float temp1;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	EXTIX_Init();
	LED_Init();
	LCD_Init();			 	
 	adc_init();		  		//ADC初始化
	ad9958_init_1();


	POINT_COLOR=RED;//设置字体为红色 

	draw_background(); 
	sweeperfre();	

	while(1)
	{
		
	}
 }

