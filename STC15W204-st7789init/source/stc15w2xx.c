/*
	stc15w204��ʼ��ST7789
	����TM 2.8''LCD 
*/
#include "stc15w2xx.h"
#include "delayms.h"

#define  CS 		P54
#define  RST 		P55  
#define  SDO 		P33 
#define  SCL 		P32 

#define  RUN 	    P31



/*CPU ���Ŷ���*/
/*-----------------------------
	CS-1<---|+++++|--8<-SDO
	3V3-2 --|	  |--7<-SCL
	RST-3<--|	  |--6
	GND-4---|+++++|--5

------------------------------*/
/*���������*/
void ClearWDT( void );
void write_command (unsigned char y);
void write_data(unsigned char w);
void LCD_Init(void);


void main(void)
{	
	EA = 0;
	RUN = 0;	
	Delay1000ms();  //��ʱ1s
	Delay1000ms();  //��ʱ1s
	ClearWDT();
	LCD_Init();
    while(1)
	   	{	
			EA = 1;
			ClearWDT();	
		}

}
			
/***********************************************************************************
�������ƣ�void clrWatchdog( void )
����������ι���Ź���ÿ9sιһ�Ρ�
����˵�����̶�9sιһ�Σ�����û��Ҫ������
����˵������
************************************************************************************/
void ClearWDT( void )
{
    WDT_CONTR = 0x37;    
}


/***NOTE:VCC=3.3V,�ڽ���RGBģʽǰ����Ҫ��SPI�������г�ʼ��********/
/*****************************д����******************************/

void write_command (unsigned char y)
{
	unsigned char i;
	CS = 0;
	SCL = 0;
	SDO = 0;
	SCL = 1;
	for(i=0;i<8;i++)
		{
		SCL = 0;      
		  if (y&0x80)
				{
			  SDO = 1;
				}
			else
				{
			  SDO = 0;
				}
		  SCL = 1;
			y=y<<1;
		}
		CS = 1;
}
/***************************д����****************************/
void write_data(unsigned char w) 
{
	unsigned char i;
	CS = 0;
	SCL = 0;
	SDO = 1;
	SCL = 1;
	for(i=0;i<8;i++)
	 {
		SCL = 0;
		 if (w&0x80)
		 {
			SDO = 1;
		 }
		 else
		 {
			SDO = 0;
		 }
	
		SCL = 1;
	   w=w<<1;
	 }
	CS = 1;
}

/****************ST7789  TM28���г�ʼ��******************/
void LCD_Init(void)
{
/*-----ST7789S reset sequence-----*/
   RST = 1;
   Delay1ms();	//��ʱ1ms
   RST = 0;
   Delay10ms();
   RST = 1;
   Delay120ms();
/*-----Enter Sleep/Standby mode-----*/
   	write_command(0x28);//display off
   	Delay10ms();
   	write_command(0x10);//sleep DY??
   	Delay120ms();
	write_command(0x11);
	Delay120ms();
	
/*--ST7789V porch control set--*/
	write_command(0xb2);
	write_data(0x0c);
	write_data(0x0c);
	write_data(0x00);
	write_data(0x33);
	write_data(0x33);
	
/**�˴���Ҫ����������֤**/	
/*--Gate Control ,VGH = 13.26V,VGL = -10.43V--*/
	write_command(0xb7);
	write_data(0x35);


/*----------ST7789V Power set------------*/
/*
	�˴���Ҫ����������֤VCOM
	VCOM = 0.625V
*/
	write_command(0xbb);
	write_data(0x15);			

/*
	LCM Control
	XMH: this bit can reverse source output order and 
	only support for RGB interface without RAM mode
*/
	write_command(0xc0);
	write_data(0x2c);

/*
	VDV and VRH Command Enable
*/
	write_command(0xc2);
	write_data(0x01);

/*
	VRH Set
	VAP(GVDD)=4.1+( vcom+vcom offset+0.5vdv)
	VAN(GVCL)= -4.1+( vcom+vcom offset-0.5vdv)
*/
	write_command(0xc3);
	write_data(0x0b);
	
/*
	VDV Set
	VDV = 0V
*/
	write_command(0xc4);
	write_data(0x20);

/*
	Frame Rate Control in Normal Mode
	FR in normal mode  = 60Hz	
	Note:
	1. Frame rate=10MHz/(320+FPA[6:0]+BPA[6:0])*(250+RTNA[4:0]*16).
	2. FPA[6:0] and BPA[6:0] are in command B2h
	3. In this frame rate table, FPA[6:0]=0Ch, BPA[6:0]=0Ch

*/
	write_command(0xc6);
	write_data(0x0f);
	
	
/*
	??����??
	Register Value Selection 2
	default 0x0f	
*/
	write_command(0xca);
	write_data(0x0f);
	

/*
	??����??
	Register Value Selection 1
	default 0x08
*/
	write_command(0xc8);
	write_data(0x08);
	
/*
	??����??
	Write Content Adaptive Brightness Control 
	and Color Enhancement
	CECTRL=1: Color Enhancement On
	
*/
	write_command(0x55);
	write_data(0x90);


/*
	�˴���Ҫ����������֤
	Power Control 1
	AVDD =6.8V,AVCL=-4.8V,VDS=2.3V
*/
	write_command(0xd0);
	write_data(0xa4);
	write_data(0xa1);

/*-----ST7789V gamma setting---------*/
/*
	�˴���Ҫ����������֤
	Positive Voltage Gamma Control
*/
	write_command(0xe0);
	write_data(0xd0);
	write_data(0x00);
	write_data(0x09);
	write_data(0x0c);
	write_data(0x0e);
	write_data(0x28);
	write_data(0x31);
	write_data(0x43);
	write_data(0x42);
	write_data(0x3a);
	write_data(0x14);
	write_data(0x15);
	write_data(0x15);
	write_data(0x1a);
	
	
/*
	�˴���Ҫ����������֤
	Negative Voltage Gamma Control
*/
	write_command(0xe1);
	write_data(0xd0);
	write_data(0x00);
	write_data(0x09);
	write_data(0x0c);
	write_data(0x0c);
	write_data(0x28);
	write_data(0x2d);
	write_data(0x45);
	write_data(0x41);
	write_data(0x3b);
	write_data(0x19);
	write_data(0x16);
	write_data(0x15);
	write_data(0x1b);

/*----SET RGB Interfae----*/
	write_command(0xb0); 	// RAM Control
	write_data(0x11); 		//set RGB interface and DE mode.
	write_data(0x00); 

	write_command(0xb1); 	// RGB Interface Control
	write_data(0xc0); 		// set DE mode ; SET Hs,Vs,DE,DOTCLK signal polarity 
	write_data(0x04); 
	write_data(0x20); 

	write_command(0x3a);  //Interface Pixel Format
	write_data(0x66);     //18 RGB ,666-18BIT RGB

/*-----Exit Sleep/Standby mode-----*/
//write_command(0x21);

	write_command(0x11);
	Delay120ms();
	write_command(0x29); //display on
}

/*
void LCD_Enter_Standby(void)
{
   write_command(0x28);//display off
   HAL_Delay(10);
   write_command(0x10);//sleep ����
}

void LCD_Exit_Standby(void)
{
	 HAL_Delay(100);
   write_command(0x11);// Exit Sleep/ Standby mode
   HAL_Delay(120);
   write_command(0x29);//display on
}
*/


