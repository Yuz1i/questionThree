#include "ad9958.h"
#include "delay.h"

void ad9958_init(void)//ready
{
		GPIO_InitTypeDef ad9958_initstructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	
	
		ad9958_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
		ad9958_initstructure.GPIO_Pin = AD9958_CS_IO | AD9958_UPDATE_IO | AD9958_SCLK_IO | 
	                                    AD9958_SDIO_IO | AD9958_RST_IO ;
		ad9958_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		
		GPIO_Init(GPIOF,&ad9958_initstructure);
	
	
		RESET_1;//进行初始化
		delay_us(3);
		RESET_0;

	
		CS_1;//把这两个线拉高
		CLCK_1;
		delay_us(3);

		
}



void ad9958_io_update(void)//yes
{
	IO_UPDATE_0;
	delay_us(20);
	IO_UPDATE_1;
	delay_us(20);
	IO_UPDATE_0;
	delay_us(20);
}

void ad9958_write_reg(uint addr,u32 data)//yes  
{  
	uint cir;
	uint judge;
	uint len; 
		
	if(addr == CSR )//标定每个寄存器的长度
	{
		len = 8;
	}
	
	else if((addr == FR2) || (addr == CPOW0) || (addr == LSRR) )
	{
		len = 16;
	}
	else if((addr == FR1) || (addr == CFR) || (addr == ACR))
	{
		len = 24;
	
	}
	else
	{
	
		len = 32;
	}
	CS_0;
	CLCK_0;
	delay_us(1);
		{
			
			for(cir = 0;cir < 8;cir ++)//传送地址
			{
				judge = (addr >> (7 - cir)) & 1;
				CLCK_0;
				delay_us(2);
				if(judge == 1)
				{
					DATA_1;
					delay_us(2);
				}
				if(judge == 0)
				{
					DATA_0;
					delay_us(2);
				}
				CLCK_1;
				delay_us(2);
			}
			
			for(cir = 0;cir < len;cir ++)//传送数据
			{
				judge = (data >> (len   - 1 - cir))&1;
				CLCK_0;
				delay_us(2);
				if(judge == 1)
				{
					DATA_1;
					delay_us(2);
				}
				if(judge == 0)
				{
					DATA_0;
					delay_us(2);
				}
				CLCK_1;
				delay_us(2);
			}

		}
		
		

		
	CS_1;
	CLCK_0;
	delay_us(1);
}

void ad9958_single_CH1_Out(u32 fre,u32 amp,u16 pha)
{

}

void ad9958_single_CH0_Out(double fre,double amp,double pha)
{
	
}

void ad9958_sin_cos_mode(uint fre,uint amp)
{

}


void ad9958_write_amplitude(uint  amp)
{ 
	ad9958_write_reg(ACR,amp);
	CS_1;
}


void ad9958_write_phase(uint phase)
{
	u16  phase_data = phase;
	phase_data = phase * 45.51111111111111111;
	ad9958_write_reg(CPOW0,phase_data);
	CS_1;
}

void ad9958_write_frequent(uint freq)
{
	unsigned int freq_data;
	freq_data = freq * 8.589934592;
	ad9958_write_reg(CFTW0,freq_data);
}




void ad9958_init_1(void)
{
	ad9958_init();
	CS_1;
	CLCK_0;
	IO_UPDATE_0;
	RESET_1;
	delay_us(20);
	RESET_0;
	ad9958_write_reg(CSR,0x40);
	ad9958_write_reg(FR1,FR1_DATA);
	ad9958_write_reg(CSR,0x80);
	ad9958_write_reg(FR1,FR1_DATA);
	ad9958_io_update();
}

long fre1,fre0;
void ad9958_scan(void)
{
	u8 t=0; 
 	u16 lastpos[5][2];		//记录最后一次的数据 
	long fre = 100;	
//	int amp0,amp1;
	int pha1 = 90;
	int pha0 = 0;
	
	fre1 = fre;
	fre0 = fre;
//    amp1 = amp0 = 839;
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
						if((y < 82) && (y > 32) )
						{
							if(x > 425)//160m
							{
								fre0 = 160000000;
								
							}
							else if(x > 365)//+100m
							{
								fre0 += 100000000;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							else if(x > 305)//+10m
							{
								fre0 += 10000000;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							else if(x > 245)//+1M
							{
								fre0 += 1000000;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							else if(x > 185)//+100k
							{
								fre0 += 100000;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							else if(x > 125)//+10k
							{
								fre0 += 10000;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							else if(x > 65)//+1k
							{
								fre0 += 1000;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							else if(x > 0)//+100
							{
								fre0 += 100;
								if(fre0 > 160000000)
								{
									fre0 = 160000000;
								}
							}
							ad9958_out(0,fre0,pha0,439);
							{
								
								LCD_ShowxNum(211,600,fre0,10,16,0);
								LCD_ShowString(100,600,110,16,16,"ch2frequent:");
							}
						}
						else if((y < 165) && (y > 115) )
						{
							if(x > 425)//0
							{
								fre0 = 0;
								
							}
							
							else if(x > 365)//-100m
							{
								 
								fre0 -= 100000000;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							
								
							}
							else if(x > 305)//-10m
							{
								fre0 -= 10000000;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							}
							else if(x > 245)//-1M
							{
								fre0 -= 1000000;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							}
							else if(x > 185)//-100k
							{
								fre0 -= 100000;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							}
							else if(x > 125)//-10k
							{
								fre0 -= 10000;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							}
							else if(x > 65)//-1k
							{
								fre0 -= 1000;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							}
							else if(x > 0)//-100
							{
								fre0 -= 100;
								if(fre0 <= 0)
								{
									fre0 = 0;
								}
							}
							ad9958_out(0, fre0 ,  pha0,  439);
							{
								
								LCD_ShowxNum(211,600,fre0,10,16,0);
								LCD_ShowString(100,600,110,16,16,"ch2frequent:");
							}
						}
						
#if 1
						
						else if((y < 302) && (y > 252) )
						{{
							if(x > 425)//160m
							{
								fre1 = 160000000;
								
							}
							else if(x > 365)//+100m
							{
								fre1 += 100000000;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							else if(x > 305)//+10m
							{
								fre1 += 10000000;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							else if(x > 245)//+1M
							{
								fre1 += 1000000;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							else if(x > 185)//+100k
							{
								fre1 += 100000;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							else if(x > 125)//+10k
							{
								fre1 += 10000;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							else if(x > 65)//+1k
							{
								fre1 += 1000;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							else if(x > 0)//+100
							{
								fre1 += 100;
								if(fre1 > 160000000)
								{
									fre1 = 160000000;
								}
							}
							ad9958_out(1, fre1 ,pha1,  439);
							{
								
								LCD_ShowxNum(211,632,fre1,10,16,0);
								LCD_ShowString(100,632,110,16,16,"ch1frequent:");
							}
						}
						}
						else if((y < 385) && (y > 336) )
						{
							{
							if(x > 425)//0
							{
								fre1 = 0;
								
							}
							
							else if(x > 365)//-100m
							{
								 
								fre1 -= 100000000;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							
								
							}
							else if(x > 305)//-10m
							{
								fre1 -= 10000000;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							}
							else if(x > 245)//-1M
							{
								fre1 -= 1000000;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							}
							else if(x > 185)//-100k
							{
								fre1 -= 100000;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							}
							else if(x > 125)//-10k
							{
								fre1 -= 10000;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							}
							else if(x > 65)//-1k
							{
								fre1 -= 1000;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							}
							else if(x > 0)//-100
							{
								fre1 -= 100;
								if(fre1 <= 0)
								{
									fre1 = 0;
								}
							}
							ad9958_out(1,fre1,pha1,439);
							{
								
								LCD_ShowxNum(211,632,fre1,11,16,0);
								LCD_ShowString(100,632,110,16,16,"ch1frequent:");
							}
							
						}
						}
						
#endif
#if 1
			else if((y < 470) && (y > 416) )
						{{
							if(x > 425)//160m
							{
								fre = 160000000;
								
							}
							else if(x > 365)//+100m
							{
								fre += 100000000;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							else if(x > 305)//+10m
							{
								fre += 10000000;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							else if(x > 245)//+1M
							{
								fre += 1000000;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							else if(x > 185)//+100k
							{
								fre += 100000;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							else if(x > 125)//+10k
							{
								fre += 10000;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							else if(x > 65)//+1k
							{
								fre += 1000;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							else if(x > 0)//+100
							{
								fre += 100;
								if(fre > 160000000)
								{
									fre = 160000000;
								}
							}
							ad9958_out(2, fre , pha0,  439);
							{
								fre0 = fre1 = fre;
								LCD_ShowxNum(211,600,fre,10,16,0);
								LCD_ShowString(100,600,110,16,16,"ch1frequent:");
								LCD_ShowxNum(220,632,fre,10,16,0);
								LCD_ShowString(100,632,110,16,16,"ch2frequent:");
								//printf("%d\r\n",fre);
							}
						}
						}
						else if((y < 560) && (y > 510) )
						{
							{
							if(x > 425)//0
							{
								fre = 0;
								
							}
							
							else if(x > 365)//-100m
							{
								 
								fre -= 100000000;
								if(fre <= 0)
								{
									fre = 0;
								}
							
								
							}
							else if(x > 305)//-10m
							{
								fre -= 10000000;
								if(fre <= 0)
								{
									fre = 0;
								}
							}
							else if(x > 245)//-1M
							{
								fre -= 1000000;
								if(fre <= 0)
								{
									fre = 0;
								}
							}
							else if(x > 185)//-100k
							{
								fre -= 100000;
								if(fre <= 0)
								{
									fre = 0;
								}
							}
							else if(x > 125)//-10k
							{
								fre -= 10000;
								if(fre <= 0)
								{
									fre = 0;
								}
							}
							else if(x > 65)//-1k
							{
								fre -= 1000;
								if(fre <= 0)
								{
									fre = 0;
								}
							}
							else if(x > 0)//-100
							{
								fre -= 100;
								if(fre <= 0)
								{
									fre = 0;
								}
							}
							ad9958_out(2, fre ,  pha0,  439);
							{
								
								LCD_ShowxNum(211,600,fre,10,16,0);
								LCD_ShowString(100,600,110,16,16,"ch1frequent:");
								LCD_ShowxNum(220,632,fre,10,16,0);
								LCD_ShowString(100,632,110,16,16,"ch2frequent:");
								//printf("%d\r\n",fre);
							}
							
						}
						}
						
#endif						
						
						
						
						//						if(lastpos[t][0] > 240)
//						{
//							if(lastpos[t][1] <= 500)
//							{
//								if(fre <= 160000000)
//								fre -= 100;
//							}
//						
//						}
//						if(lastpos[t][1] > 500)
//						{
//							fre = 100;
//						}
						
						//printf("fre: %ld  x :%d,y:%d\r\n",fre,x,y);
				    }
					
					{
//						LCD_ShowString(100,286,360,16,16,"-                                     +");
//						LCD_ShowxNum(220,400,fre,56,16,1);
//						LCD_ShowString(224,240,64,16,16,"frequent");
//						
//						ad9958_out(1,fre,pha1,amp1);
//						ad9958_out(0,fre0,pha0,amp0);
					}
					
//					lastpos[t][0]=tp_dev.x[t];
//					lastpos[t][1]=tp_dev.y[t];
					//if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<16)
					{
//						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
	}	
	
}

void ad9958_out(int chanel,int fre , int pha, int amp)
{
	if(chanel == 1)
	{
		ad9958_write_reg(CSR,0x80); //选择哪个通道
		ad9958_write_amplitude(amp);
		ad9958_write_frequent(fre);
		ad9958_write_phase(pha);___
	}
	else if(chanel == 0)
	{
		ad9958_write_reg(CSR,0x40); //选择哪个通道
		ad9958_write_amplitude(amp);
		ad9958_write_frequent(fre);
		ad9958_write_phase(pha);___
	}
	else if(chanel == 2)
	{
		ad9958_write_reg(CSR,0x40); //选择哪个通道
		ad9958_write_amplitude(amp);
		ad9958_write_frequent(fre);
		ad9958_write_phase(pha);
		ad9958_write_reg(CSR,0x80); //选择哪个通道
		ad9958_write_amplitude(amp);
		ad9958_write_frequent(fre);
		ad9958_write_phase(pha+90);___
	}

}
void ad9958_draw(void)
{
	
}
