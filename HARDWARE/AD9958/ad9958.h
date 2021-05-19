
#ifndef __ad9958_h
#define __ad9958_h
///////////////////////////////////////////////////头文件区/////////////////////////////////////////////////
#include "sys.h" 
#include "lcd.h"
#include "touch.h"
#include "led.h"

/////////////////////////////////////////////////宏定义区//////////////////////////////////////////////////

//定义各个引脚
#define AD9958_UPDATE           PFout(0)
#define AD9958_CS               PFout(1)
#define AD9958_SCLK             PFout(2)
#define AD9958_SDIO             PFout(3)
#define AD9958_RST              PFout(4)
//#define AD9958_DGND             PEout(12)

#define AD9958_UPDATE_IO        GPIO_Pin_0
#define AD9958_CS_IO            GPIO_Pin_1
#define AD9958_SCLK_IO          GPIO_Pin_2
#define AD9958_SDIO_IO          GPIO_Pin_3
#define AD9958_RST_IO           GPIO_Pin_4
//#define AD9958_DGND_IO          GPIO_Pin_12


//定义寄存器的地址
#define CSR                     0x00
#define FR1                     0x01
#define FR2                     0x02
#define CFR		                0x03
#define CFTW0                   0x04
#define CPOW0                   0x05
#define ACR                     0x06
#define LSRR                    0x07
#define RDW                     0x08
#define FDW                     0x09



#define CW1                     0x0A
#define CW2                     0x0B
#define CW3                     0x0C
#define CW4                     0x0D
#define CW5                     0x0E
#define CW6                     0x0F
#define CW7                     0x10
#define CW8                     0x11
#define CW9                     0x12
#define CW10                    0x13
#define CW11                    0x14
#define CW12                    0x15
#define CW13                    0x16
#define CW14                    0x17
#define CW15                    0x18


//定义位操作
#define RESET_1                 (AD9958_RST = 1)
#define RESET_0                 (AD9958_RST = 0)     

#define DATA_1                  (AD9958_SDIO = 1)
#define DATA_0                  (AD9958_SDIO = 0)

#define CLCK_1                  (AD9958_SCLK = 1)
#define CLCK_0                  (AD9958_SCLK = 0)

#define CS_1                    (AD9958_CS = 1)
#define CS_0                    (AD9958_CS = 0)

#define IO_UPDATE_1             (AD9958_UPDATE = 1)
#define IO_UPDATE_0             (AD9958_UPDATE = 0)


#define uint                     unsigned int
#define uchar                    unsigned char
	
#define LSB                      0x01//使用时数据相或
#define MSB                      0x00//默认值

#define ___                      ad9958_io_update();


//      default data
#define FR1_DATA                0xd00000  ///0xd00000   25Mhz      0xa80000 50Mhz
#define CFR_DATA                0x300
#define CSR_DATA                0xf0
#define ACR_DATA                0xffd3ff
//////////////////////////////////////////////////函数区/////////////////////////////////////////////////////////////

void ad9958_init(void);//

void ad9958_write_addr(uint addr);
void ad9958_io_update(void);
void ad9958_write_reg(uint addr,u32 data);
void ad9958_read_reg(u8 addr);


void ad9958_single_tone_setting(u32 fre,u16 amp);
void ad9958_single_CH1_out(double fre,double amp,double pha);
void ad9958_single_CH0_out(double fre,double amp,double pha);

void ad9958_sweeper_fre_setting(u32 fre_start,u32 fre_end,u16 amp,u16 pha);
void ad9958_start_sweep(void);
  
void ad9958_tran_direct(uint direction);//direction belong to LSB or MSB


void ad9958_sin_cos_mode(uint fre,uint amp);
void ad9958_write_amplitude(uint  amp);
void ad9958_write_phase(uint phase);
void ad9958_write_frequent(uint freq);

void test(void);
void ad9958_init_1(void);

void ad9958_control(void);
void ad9958_scan(void);

void ad9958_out(int chanel,int fre , int pha, int amp);
void ad9958_draw(void);
///////////////////////////////////////////////////变量定义///////////////////////////////////////////////////////////











#endif


