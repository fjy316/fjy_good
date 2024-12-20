/*
 * oled.c
 *
 *  Created on: 2022年7月24日
 *      Author: Unicorn_Li
 */
#include "oled.h"
#include "oledfont.h"
#include <string.h>
#include "math.h"
#include "stdlib.h"

/**********************************************************
 * 初始化命令,根据芯片手册书写，详细步骤见上图以及注意事项
 ***********************************************************/
uint8_t CMD_Data[]={
0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,0xA1, 0xC8, 0xDA,

0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,0x8D, 0x14,

0xAF};

extern   uint16_t pre_j;
uint8_t Grame[8][128] = {0};
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

/**
 * @function: void OLED_Init(void)
 * @description: OLED初始化
 * @return {*}
 */
void OLED_Init(void)
{
	HAL_Delay(200);

	uint8_t i = 0;
	for(i=0; i<23; i++)
	{
		OLED_WR_CMD(CMD_Data[i]);
	}
// OLED_StatUpdate( );
//  HAL_Delay(200);
//	
//	OLED_WR_CMD(0xAE); //开显示
//	OLED_WR_CMD(0x20);	//设置内存寻址模式	
//	
//	OLED_WR_CMD(0x10);	//00，水平寻址模式;01，垂直寻址模式;10，页面寻址模式(重置);11，无效
//	OLED_WR_CMD(0xb0);	//为页面寻址模式设置页面开始地址，0-7
//	OLED_WR_CMD(0x00); //---设置低列地址
//	OLED_WR_CMD(0x10); //---设置高列地址

//	OLED_WR_CMD(0xc8);	//设置COM输出扫描方向
//	OLED_WR_CMD(0x40); //--设置起始行地址
//	OLED_WR_CMD(0x81); //--set contrast control register
//	OLED_WR_CMD(0xff); //亮度调节 0x00~0xff
//	OLED_WR_CMD(0xa1); //--设置段重新映射0到127
//	OLED_WR_CMD(0xa6); //--设置正常显示
//	OLED_WR_CMD(0xa8); //--设置复用比(1 ~ 64)
//	OLED_WR_CMD(0x3F); //
//	OLED_WR_CMD(0xa4); //0xa4,输出遵循RAM内容;0xa5,Output忽略RAM内容
//	OLED_WR_CMD(0xd3); //-设置显示抵消
//	OLED_WR_CMD(0x00); //-not offset
//	OLED_WR_CMD(0xd5); //--设置显示时钟分频/振荡器频率
//	OLED_WR_CMD(0xf0); //--设置分率
//	OLED_WR_CMD(0xd9); //--设置pre-charge时期
//	OLED_WR_CMD(0x22); //
//	OLED_WR_CMD(0xda); //--设置com大头针硬件配置
//	OLED_WR_CMD(0x12);
//	OLED_WR_CMD(0xdb); //--设置vcomh
//	OLED_WR_CMD(0x20); //0x20,0.77xVcc
//	OLED_WR_CMD(0x8d); //--设置DC-DC
//	OLED_WR_CMD(0x14); //
//	OLED_WR_CMD(0xaf); //--打开oled面板


//	OLED_WR_CMD(0xAE); //关闭显示
//	OLED_WR_CMD(0xD5); //设置时钟分频因子,震荡频率
//	OLED_WR_CMD(80);   //[3:0],分频因子;[7:4],震荡频率
//	OLED_WR_CMD(0xA8); //设置驱动路数
//	OLED_WR_CMD(0X3F); //默认0X3F(1/64) 
//	OLED_WR_CMD(0xD3); //设置显示偏移
//	OLED_WR_CMD(0X00); //默认为0

//	OLED_WR_CMD(0x40); //设置显示开始行 [5:0],行数.
//													    
//	OLED_WR_CMD(0x8D); //电荷泵设置
//	OLED_WR_CMD(0x14); //bit2，开启/关闭
//	OLED_WR_CMD(0x20); //设置内存地址模式
//	OLED_WR_CMD(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
//	OLED_WR_CMD(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
//	OLED_WR_CMD(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
//	OLED_WR_CMD(0xDA); //设置COM硬件引脚配置
//	OLED_WR_CMD(0x12); //[5:4]配置
//		 
//	OLED_WR_CMD(0x81); //对比度设置
//	OLED_WR_CMD(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
//	OLED_WR_CMD(0xD9); //设置预充电周期
//	OLED_WR_CMD(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
//	OLED_WR_CMD(0xDB); //设置VCOMH 电压倍率
//	OLED_WR_CMD(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
//  OLED_WR_CMD(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
//	OLED_WR_CMD(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
//	OLED_WR_CMD(0xAF); //开启显示	 
//	OLED_Clear();

}

/**
 * @function: void OLED_WR_CMD(uint8_t cmd)
 * @description: 向设备写控制命令
 * @param {uint8_t} cmd 芯片手册规定的命令
 * @return {*}
 */
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}

/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: 向设备写控制数据
 * @param {uint8_t} data 数据
 * @return {*}
 */
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}

/**
 * @function: void OLED_On(void)
 * @description: 更新显示

 * @return {*}
 */
void OLED_On(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(1);
	}
}


/**
 * @function: OLED_Clear(void)
 * @description: 清屏,整个屏幕是黑色的!和没点亮一样!!!
 * @return {*}
 */
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	}
}

/**
 * @function: void OLED_Display_On(void)
 * @description: 开启OLED显示
 * @return {*}
 */
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON,打开显示
}


/**
 * @function: void OLED_Display_Off(void)
 * @description: 关闭OLED显示
 * @return {*}
 */
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF，关闭显示
}

/**
 * @function: void OLED_Set_Pos(uint8_t x, uint8_t y)
 * @description: 坐标设置
 * @param {uint8_t} x,y
 * @return {*}
 */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_CMD(0xb0+y);	//设置页地址（0~7）
	OLED_WR_CMD(((x&0xf0)>>4)|0x10); //设置显示位置—列高地址
	OLED_WR_CMD(x&0x0f);	//设置显示位置—列低地址
}


/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n函数
 * @param {uint8_t} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}

/**
 * @function: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size,uint8_t Color_Turn)
 * @description: 在OLED12864特定位置开始显示一个字符
 * @param {uint8_t} x字符开始显示的横坐标
 * @param {uint8_t} y字符开始显示的纵坐标
 * @param {uint8_t} chr待显示的字符
 * @param {uint8_t} Char_Size待显示字符的字体大小,选择字体 16/12
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t Color_Turn)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				{
				  if(Color_Turn)
					  OLED_WR_DATA(~F8X16[c*16+i]);
				  else
					  OLED_WR_DATA(F8X16[c*16+i]);
				}
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~F8X16[c*16+i+8]);
				  else
					  OLED_WR_DATA(F8X16[c*16+i+8]);
			    }

			}
	     else
	     {
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~F6x8[c][i]);
				  else
					  OLED_WR_DATA(F6x8[c][i]);
			    }
		  }
}

/**
 * @function: void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_tChar_Size, uint8_t Color_Turn)
 * @description: 在OLED12864特定位置开始显示字符串
 * @param {uint8_t} x待显示字符串的开始横坐标x:0~127
 * @param {uint8_t} y待显示字符串的开始纵坐标 y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {uint8_t} *chr待显示的字符串
 * @param {uint8_t} Char_Size待显示字符串的字体大小,选择字体 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowString(uint8_t x,uint8_t y,char*chr,uint8_t Char_Size, uint8_t Color_Turn)
{
	uint8_t  j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size, Color_Turn);
			if (Char_Size == 12) //6X8的字体列加6，显示下一个字符
				x += 6;
			else  //8X16的字体列加8，显示下一个字符
				x += 8;

			if (x > 122 && Char_Size==12) //TextSize6x8如果一行不够显示了，从下一行继续显示
			{
				x = 0;
				y++;
			}
			if (x > 120 && Char_Size== 16) //TextSize8x16如果一行不够显示了，从下一行继续显示
			{
				x = 0;
				y++;
			}
			j++;
	}
}

/**
 * @function: void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, Color_Turn)
 * @description: 显示数字
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {unsigned int} num:输入的数据
 * @param {uint8_t } len:输入的数据位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
}


/**
 * @function: void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
 * @description: 显示正负浮点数
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {float} num:输入的浮点型数据
 * @param {uint8_t } z_ len:整数部分的位数
 * @param {uint8_t } f_len: 小数部分的位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp,i=0;//i为负数标志位
	uint8_t enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//整数部分
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2, Color_Turn);

	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2, Color_Turn);
	}
	if(i==1)//如果为负，就将最前的一位赋值‘-’
	{
		OLED_ShowChar(x,y,'-',size2, Color_Turn);
		i=0;
	}
}



/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: 在OLED特定位置开始显示16X16汉字
 * @param {uint8_t} x待显示的汉字起始横坐标x: 0~112，两列汉字之间需要间隔16
 * @param {uint8_t} y待显示的汉字起始纵坐标 y: 0~6 , 两行汉字之间需要间隔2
 * @param {uint8_t} no待显示的汉字编号
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no][t]); //显示汉字的上半部分
				else
					OLED_WR_DATA(Hzk[2*no][t]); //显示汉字的上半部分
        }

		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no+1][t]); //显示汉字的下半部分
				else
					OLED_WR_DATA(Hzk[2*no+1][t]);//显示汉字的下半部分

         }
}

/**
 * @function: void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域显示BMP图片
 * @param {uint8_t} x0图像开始显示横坐标  x0:0~127
 * @param {uint8_t} y0图像开始显示纵坐标  y0:0~7
 * @param {uint8_t} x1图像结束显示横坐标  x1:1~128
 * @param {uint8_t} y1图像结束显示纵坐标  y1:1~8
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
{
   uint32_t j = 0;
   uint8_t x = 0, y = 0;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
		{
			if (Color_Turn)
				OLED_WR_DATA(~BMP[j++]);//显示反相图片
			else
				OLED_WR_DATA(BMP[j++]);//显示图片

		}
	}
}
/**
 * @function: void OLED_DrawNengLiangtiao(uint8_t num, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域显示能量条
 * @param {uint8_t} y0图像开始显示纵坐标  y0:0~7
 * @param {uint8_t} y1图像结束显示纵坐标  y1:1~8
 * @param {uint8_t} num能量条的长度 num<42
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
/*num<=42*/
//左能量条
void OLED_DrawNengLiangtiao( uint8_t y0, uint8_t y1,uint8_t num, uint8_t *  BMP,uint8_t Color_Turn)//能量条从左到右增加
{
	
   uint32_t j = 0;
   uint8_t x = 0, y = 7;
	for(y=y0;y<y1;y++)
	{
	  OLED_Set_Pos(0,y);
    for(x=0;x<56;x++)
		{
				if((x>=0&&x<7)||(x>48&&x<=55))
				{
					if (Color_Turn)
						OLED_WR_DATA(BMP[x]);//显示能量条不变的部分
					else
					  OLED_WR_DATA(~BMP[x]);//显示能量条不变的部分(反相)
				}
				else 
				{
					if(x<7+num)
						OLED_WR_DATA(0xDB);//显示有值能量条	
					else
						OLED_WR_DATA(0xC3);//显示空能量条		
				}
			}	
	}
/*能量条*/
//unsigned char BMP37[]={
//0xC3,0x81,0x00,0x00,0x00,0x18,0x3C,0x24,0x24,0x24,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,
//0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,
//0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,
//0x3C,0x3C,0x18,0x00,0x00,0x00,0x81,0xC3,/*"未命名文件",0*/
/* (56 X 8 )*/
//};

}

/**
 * @function: void OLED_DrawNengLiangtiao(uint8_t num, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域0<x<56显示能量条
 * @param {uint8_t} y0图像开始显示纵坐标  y0:0~7
 * @param {uint8_t} y1图像结束显示纵坐标  y1:1~8
 * @param {uint8_t} num能量条的长度 num<42
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
/*num<=42*/
//右能量条
void OLED_DrawNengLiangtiao_1(uint8_t y0, uint8_t y1,uint8_t num, uint8_t *  BMP,uint8_t Color_Turn)//能量条从右到左增加
{
	
   uint32_t j = 0;
   uint8_t x = 0, y = 7;
	
	
	for(y=y0;y<y1;y++)
	{
	  OLED_Set_Pos(71,y);
    for(x=0;x<56;x++)
		{
				if((x>=0&&x<7)||(x>48&&x<=55))
				{
					if (Color_Turn)
						OLED_WR_DATA(BMP[x]);//显示能量条不变的部分
					else
					  OLED_WR_DATA(~BMP[x]);//显示能量条不变的部分(反相)
				}
				else 
				{
//					if(x<7+num)
					if(x>48-num)
						OLED_WR_DATA(0xDB);//显示有值能量条	
					else
						OLED_WR_DATA(0xC3);//显示空能量条		
				}
			}
	}

}
/************************Fast********************************/
/**
 * @function: uint8_t Draw_pot(uint8_t x,uint8_t y)
 * @description:   画点,左下角为原点     
 * @param {uint8_t} x    点的横坐标0~127
 * @param {uint8_t} y    点的纵坐标0~63
 * @return {*}
 */

uint8_t Draw_pot(uint8_t x,uint8_t y)
{
	uint8_t y_page,y_dex;
	if(x<0 || x>127 || y<0 || y>63)return 1;//³¬³ö·¶Î§ÁË.
	if(y >= 64)
		return 1;
	y_page = ( 63 - y ) / 8;
	y_dex = ( 63 - y ) % 8;
	Grame[y_page][x] |= 0x01 << y_dex;
	

	
	return 0;
}


/**
 * @function: uint8_t Clear_pot(uint8_t x,uint8_t y)
 * @description:   清除点,左下角为原点     
 * @param {uint8_t} x    点的横坐标0~127
 * @param {uint8_t} y    点的纵坐标0~63
 * @return {*}
 */
uint8_t Clear_pot(uint8_t x,uint8_t y)
{
	uint8_t y_page,y_dex;
	if(y >= 64)
		return 1;
	y_page = ( 63 - y ) / 8;
	y_dex = ( 63 - y ) % 8;
	Grame[y_page][x] &= ~( 0x01 << y_dex );
	
	return 0;
}
/**
 * @function:static uint8_t Pos_Target(uint8_t *x,uint8_t x_target)
 * @description:   使x通过自加或自减接近x_target     
 * @param {uint8_t} *x        当前x值
 * @param {uint8_t} x_target  目标x值
 * @return {*}
 */

static uint8_t Pos_Target(uint8_t *x,uint8_t x_target)
{
	uint8_t i;
	i = (*x);
	if((*x) < x_target)
	{
		(*x)++;
	}
	else
	{
		(*x)--;
	}
	return i;
}
/**************************************************************************/
/**
 * @function: void line(uint8_t x,uint8_t y,uint8_t length,uint8_t mode)
 * @description:   画水平、垂直线，左下角为坐标原点     
 * @param {uint8_t} x        起点横坐标0~127
 * @param {uint8_t} y        起点纵坐标0~63
 * @param {uint8_t} length   长度  0~127
 * @param {uint8_t} mode     1 向右 ，2 向上 ,3向左  ,4向下 5,向右虚线 ，6 向上虚线 ,7向左虚线  ,8向下虚线
 * @return {*}
 */
void line(long x,long y,char length,uint8_t mode)
{
	if(mode == 1)
	{
		
		for(long x1=x;x1<x+length;x1++)//向右
		{
			if(x1>=0&&x1<=127)
			  Draw_pot(x1,y);
		}
	}
	else if (mode == 2)
	{
		 for(long y1=y;y1<y+length;y1++)//向上
		{
			 if(x>=0&&x<=127)
			  Draw_pot(x,y1);
		}
	}	
	else if (mode == 3)
	{
		 for(long x1=x;x1>x-length;x1--)//向左
		{
			if(x1>=0&&x1<=127)
			 Draw_pot(x1,y);
		}
	}
	else if (mode == 4)
	{
		 for(long y1=y;y1>y-length;y1--)//向下
		{
			if(x>=0&&x<=127)
			 Draw_pot(x,y1);
		}
	}
	else if(mode == 5)
	{
		for(long x1=x;x1<x+length;x1++)//向右 虚线
		{
			if(x1>=0&&x1<=127)
			{
				if(x1%2==0)
					Draw_pot(x1,y);
			}
	}
	}
	else if (mode == 6)
	{
		 for(long y1=y;y1<y+length;y1++)//向上 虚线
			if(x>=0&&x<=127)
			{
				 if(y1%2==0)
					 Draw_pot(x,y1);
			 }
	}	
	else if (mode == 7)
	{
		 for(long x1=x;x1>x-length;x1--)//向左 虚线
			if(x1>=0&&x1<=127)
			{		
		   if(x1%2==0)
			 Draw_pot(x1,y);
		  }
	}	
	else if(mode==8)
	{
		 for(long y1=y;y1>y-length;y1--)//向下 虚线
		{
			if(x>=0&&x<=127)
			{					
				if(y1%2==0)
					Draw_pot(x,y1);
			}				
		}
	}
	
}
/**
 * @function: void Clear_line(uint8_t x,uint8_t y,uint8_t length,uint8_t mode)
 * @description:   清除水平、垂直线，左下角为坐标原点     
 * @param {uint8_t} x        起点横坐标0~127
 * @param {uint8_t} y        起点纵坐标0~63
 * @param {uint8_t} length   长度  0~127
 * @param {uint8_t} mode     1 向右 ，2 向上 
 * @return {*}
 */
void Clear_line(uint8_t x,uint8_t y,uint8_t length,uint8_t mode)
{
	if(mode == 1)
		for(uint8_t x1=x;x1<x+length;x1++)
			Clear_pot(x1,y);
	else
		for(uint8_t y1=y;y1<y+length;y1++)
			Clear_pot(x,y1);
}
/**************************************************************************/
/**
 * @function: void drawline(float x1,float y1,float x2,float y2)
 * @description:   画一次函数(直线)，左下角为原点，斜率太大会变虚线，2点坐标画线，次完美的线                  
 * @param {float} x1 起点横坐标0~127
 * @param {float} y1 起点纵坐标0~63
 * @param {float} x2   终点横坐标0~127
 * @param {float} y2   终点纵坐标0~63
 * @return {*}
 */
void drawline(float x1,float y1,float x2,float y2)
{
	uint8_t y;
	if(x1==x2)
		if(y1<y2)
			for(uint8_t i=y1;i<y2;i++)
				Draw_pot(x1,i);
		else
			for(uint8_t i=y1;i>y2;i--)
				Draw_pot(x1,i);
	else
		if(x1<x2)
			for(uint8_t x=x1;x<x2;x++)
			{
				y=(y2-y1)/(x2-x1)*x+y1-(y2-y1)/(x2-x1)*x1;
				Draw_pot(x,y);
			}
		else
			for(uint8_t x=x1;x>x2;x--)
			{
				y=(y2-y1)/(x2-x1)*x+y1-(y2-y1)/(x2-x1)*x1;
				Draw_pot(x,y);
			}
}
/**
 * @function: void Clear_drawline(float x1,float y1,float x2,float y2)
 * @description:   画一次函数(直线)，左下角为原点，斜率太大会变虚线，2点坐标画线，次完美的线                  
 * @param {float} x1 起点横坐标0~127
 * @param {float} y1 起点纵坐标0~63
 * @param {float} x2   终点横坐标0~127
 * @param {float} y2   终点纵坐标0~63
 * @return {*}
 */

void Clear_drawline(float x1,float y1,float x2,float y2)
{
	uint8_t y;
	if(x1==x2)
		if(y1<y2)
			for(uint8_t i=y1;i<y2;i++)
				Clear_pot(x1,i);
		else
			for(uint8_t i=y1;i>y2;i--)
				Clear_pot(x1,i);
	else
		if(x1<x2)
			for(uint8_t x=x1;x<x2;x++)
			{
				y=(y2-y1)/(x2-x1)*x+y1-(y2-y1)/(x2-x1)*x1;
				Clear_pot(x,y);
			}
		else
			for(uint8_t x=x1;x>x2;x--)
			{
				y=(y2-y1)/(x2-x1)*x+y1-(y2-y1)/(x2-x1)*x1;
				Clear_pot(x,y);
			}
}

/**************************************************************************/
/**
 * @function: void OLED_DrawLine(int x0, int y0, int x1, int y1)
 * @description:   画一次函数(直线)，左下角为原点，不会出现虚线，2点坐标画线，最完美的线                  
 * @param {int} x0 起点横坐标0~127
 * @param {int} y0 起点纵坐标0~63
 * @param {int} x1   终点横坐标0~127
 * @param {int} y1   终点纵坐标0~63
 * @return {*}
 */
void OLED_DrawLine(int x0, int y0, int x1, int y1)
{
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int erro = (dx > dy ? dx : -dy) / 2;
  while(Draw_pot(x0,y0), x0 != x1 || y0 != y1)
	{
      int e2 = erro;
      if(e2 > -dx) { erro -= dy; x0 += sx;}
      if(e2 <  dy) { erro += dx; y0 += sy;}
  } 
}
/**
 * @function: uint8_t Clear_OLED_DrawLine(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
 * @description:   清除一次函数(直线)，左下角为原点，不会出现虚线，2点坐标画线，最完美的线                  
 * @param {int} x0 起点横坐标0~127
 * @param {int} y0 起点纵坐标0~63
 * @param {int} x1   终点横坐标0~127
 * @param {int} y1   终点纵坐标0~63
 * @return {*}
 */
void Clear_OLED_DrawLine(int x0, int y0, int x1, int y1)
{
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int erro = (dx > dy ? dx : -dy) / 2;
  while(Clear_pot(x0,y0), x0 != x1 || y0 != y1)
	{
      int e2 = erro;
      if(e2 > -dx) { erro -= dy; x0 += sx;}
      if(e2 <  dy) { erro += dx; y0 += sy;}
  } 
}

/**************************************************************************/
/**
 * @function: uint8_t Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
 * @description:   画一次函数(直线)，左下角为原点，斜率太大会出现虚线，
                   且没有斜率为负的判断，2点坐标画线
 * @param {uint8_t} x_start 起点横坐标0~127
 * @param {uint8_t} y_start 起点纵坐标0~63
 * @param {uint8_t} x_end   终点横坐标0~127
 * @param {uint8_t} y_end   终点纵坐标0~63
 * @return {*}
 */
uint8_t Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{
	uint8_t x = x_start;
	uint8_t y = y_start;
	
	float x_dex = x_end - x_start;
	float y_dex = y_end - y_start;

	float k = y_dex / x_dex;
	float b = (float)( (float)y_start - (float)( k * (float)x_start ) );
	
	if(x_start != x_end)
	{
		while(Pos_Target(&x,x_end) != x_end)
		{
			Draw_pot(x ,y);
			y = (uint8_t)( (float)x * k + b );
		}
	}
	else
	{
		while(Pos_Target(&y_start,y_end) != y_end)
		{
			Draw_pot(x_start,y_start);
		}
	}
	return 0;
}
/**
 * @function: uint8_t Clear_Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
 * @description:   清除一次函数(直线)，左下角为原点，斜率太大会出现虚线，
                   且没有斜率为负的判断，2点坐标画线
 * @param {uint8_t} x_start 起点横坐标0~127
 * @param {uint8_t} y_start 起点纵坐标0~63
 * @param {uint8_t} x_end   终点横坐标0~127
 * @param {uint8_t} y_end   终点纵坐标0~63
 * @return {*}
 */
uint8_t Clear_Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{
	uint8_t x = x_start;
	uint8_t y = y_start;
	
	float x_dex = x_end - x_start;
	float y_dex = y_end - y_start;

	float k = y_dex / x_dex;
	float b = (float)( (float)y_start - (float)( k * (float)x_start ) );
	
	if(x_start != x_end)
	{
		while(Pos_Target(&x,x_end) != x_end)
		{
			Clear_pot(x ,y);
			y = (uint8_t)( (float)x * k + b );
		}
	}
	else
	{
		while(Pos_Target(&y_start,y_end) != y_end)
		{
			Clear_pot(x_start,y_start);
		}
	}
	return 0;
}
/**************************************************************************/
/**
 * @function: void Draw_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t width)
 * @description:   画矩形，左下角为原点，从左到右为长，从下到上为宽
 * @param {uint8_t} x_start 起点横坐标0~127
 * @param {uint8_t} y_start 起点纵坐标0~63
 * @param {uint8_t} length  长0~128
 * @param {uint8_t} width   宽0~64
 * @return {*}
 */
void Draw_square(long x_start,long y_start,uint8_t length,uint8_t width)
{
//	width = width - 1;
	line(x_start,y_start,length,1);//
	
	line(x_start,y_start,width,2);//
	
	line(x_start + length-1,y_start,width,2);
	
	line(x_start,y_start+width,length,1);
}
/**
 * @function: void Clear_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t width)
 * @description:   清除矩形，左下角为原点，从左到右为长，从下到上为宽
 * @param {uint8_t} x_start 起点横坐标0~127
 * @param {uint8_t} y_start 起点纵坐标0~63
 * @param {uint8_t} length  长0~128
 * @param {uint8_t} width   宽0~64
 * @return {*}
 */
void Clear_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t width)
{
	Clear_line(x_start,y_start,length,1);//
	
	Clear_line(x_start,y_start,width,2);//
	
	Clear_line(x_start + length-1,y_start,width,2);
	
	Clear_line(x_start,y_start+width,length,1);
}
/**************************************************************************/

/**
 * @function: void OLED_DrawCircle_lest(uint8_t x0,uint8_t y0,uint8_t r)
 * @description: 空心圆，左下角原点
 * @param {uint8_t} x0  圆心横坐标0~127
 * @param {uint8_t} y0  圆心纵坐标0~63
 * @param {uint8_t} r  半径0~128
 * @return {*}
 */
void OLED_DrawCircle_lest(int x,int y,uint8_t  r)//不够圆的圆
{
	int x1,y1,a=0;
	float c;
	for(c=0.f;c<2*3.14f;c+=0.04f)
	{
		if(a==0)
		{
			x1=x+r*cos(c)-1; y1=y+r*sin(c);
			Draw_pot(x1,y1);
			a++;
		}
		else
		{
			x1=x+r*cos(c); y1=y+r*sin(c);
			Draw_pot(x1,y1);
		}
	}
}
/**
 * @function: void OLED_DrawCircle_better(uint8_t x0,uint8_t y0,uint8_t r)
 * @description: 空心圆，左下角原点
 * @param {uint8_t} x0  圆心横坐标0~127
 * @param {uint8_t} y0  圆心纵坐标0~63
 * @param {uint8_t} r  半径0~128
 * @return {*}
 */
void OLED_DrawCircle_better(int x0,int y0,uint8_t r)//较为圆的圆
{
	int8_t x = r;
	int8_t y = 0;
	int8_t err = 0;
	while (x >= y)
	{
		Draw_pot(x0 + x, y0 + y);
		Draw_pot(x0 - x, y0 + y);
		Draw_pot(x0 + x, y0 - y);
		Draw_pot(x0 - x, y0 - y);
		Draw_pot(x0 + y, y0 + x);
		Draw_pot(x0 - y, y0 + x);
		Draw_pot(x0 + y, y0 - x);
		Draw_pot(x0 - y, y0 - x);
		if (err <= 0)
		{
			y++;
			err += 2*y+1;
		}
		if (err > 0)
		{
			x--;
			err -= 2*x+1;
		}
	}
}
/**
 * @function: void OLED_DrawCircle_most(uint8_t x0,uint8_t y0,uint8_t r)
 * @description: 空心圆，左下角原点
 * @param {uint8_t} x0  圆心横坐标0~127
 * @param {uint8_t} y0  圆心纵坐标0~63
 * @param {uint8_t} r  半径0~128
 * @return {*}
 */
void OLED_DrawCircle_most(uint8_t x0,uint8_t y0,uint8_t r)//最圆的圆
{
	int x = 0, y = r,d;
	d = 3- 2*r;
	while (x <= y) 
	{
		Draw_pot(x0+x,y0+y);
		Draw_pot(x0-x,y0+y);
		Draw_pot(x0+x,y0-y);
		Draw_pot(x0-x,y0-y);
		Draw_pot(x0+y,y0+x);
		Draw_pot(x0-y,y0+x);
		Draw_pot(x0+y,y0-x);
		Draw_pot(x0-y,y0-x);
		if (d < 0)
			d = d+4*x+6;
		else 
		{
			d = d+4*(x-y)+10;
			y--;
		}
		x++;
	}
}
/**
 * @function: void OLED_DrawFullCircle(uint8_t x0,uint8_t y0,uint8_t r)
 * @description: 实心圆，左下角原点
 * @param {uint8_t} x0  圆心横坐标0~127
 * @param {uint8_t} y0  圆心纵坐标0~63
 * @param {uint8_t} r  半径0~128
 * @return {*}
 */
void OLED_DrawFullCircle(uint8_t x0,uint8_t y0,uint8_t r)
{
	int x = 0, y = r,yi,d;
	d = 3- 2*r;
	while (x <= y) 
		{
			for (yi = x; yi <= y; yi ++)
			{
				Draw_pot(x0+x,y0+yi);
				Draw_pot(x0-x,y0+yi);
				Draw_pot(x0+x,y0-yi);
				Draw_pot(x0-x,y0-yi);
				Draw_pot(x0+yi,y0+x);
				Draw_pot(x0-yi,y0+x);
				Draw_pot(x0+yi,y0-x);
				Draw_pot(x0-yi,y0-x);
			}
			if (d < 0)
				d = d + 4 * x + 6;
			else 
			{
				d = d + 4 * (x - y) + 10;
				y --;
			}
			x ++;
		}
}

/********************************************************/
/**
 * @function: void progress_right(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
 * @description: 向右进度条，左下角起始坐标(0,0)
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} length 0~128
 * @param {uint8_t} width  0~64
 * @param {uint8_t} value  变量，最大值要与length相等
 * @return {*}
 */
void progress_right(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
{
	uint8_t n=x+5;
	/*进度条两个边框*/
	Draw_square(n,y,length+4,width);
	Draw_square(n-1,y-1,length+6,width+2);
	/*进度条两边的四条竖线*/	
	line(n-3,y,width+1,2);
	line(n-5,y+1,width-1,2);
	line(n+length+6,y,width+1,2);
	line(n+length+8,y+1,width-1,2);
	/*进度条中间可变动的线*/
	for(uint8_t y1=y+2;y1<y+width-1;y1++)
	{
		 uint8_t change;
		 change=value-1;
	   line(n+2,y1,value,1);
		
	}
}
/**
 * @function: void progress_left(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
 * @description: 向左进度条，左下角起始坐标(0,0)
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} length 0~128
 * @param {uint8_t} width  0~64
 * @param {uint8_t} value  变量，最大值要与length相等
 * @return {*}
 */
void progress_left(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
{
	uint8_t n=x+6;
	/*进度条两个边框*/
	Draw_square(n-1,y,length+4,width);
	Draw_square(n-2,y-1,length+6,width+2);
	/*进度条两边的四条竖线*/
	line(n-4,y,width+1,2);
	line(n-6,y+1,width-1,2);
	line(n+length+5,y,width+1,2);
	line(n+length+7,y+1,width-1,2);
	/*进度条中间可变动的线*/
	for(uint8_t y1=y+2;y1<y+width-1;y1++)
	{
		 uint8_t change;
		 change=value-1;
	   line(n+length,y1,value,3);
		
	}
}
/**
 * @function: void progress_up(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
 * @description: 向上进度条，左下角起始坐标(0,0)
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} length 中间可变条长度0~128
 * @param {uint8_t} width  0~64
 * @param {uint8_t} value  变量，最大值要与length相等
 * @return {*}
 */
void progress_up(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
{
	uint8_t n=x+5;
	/*进度条两个边框*/
	Draw_square(x,y,width,length+5);
	Draw_square(x+1,y+1,width-2,length+3);
	/*进度条两边的四条竖线*/	
	line(x+1,y-2,width-2,1);
	line(x+2,y-4,width-4,1);
	line(x+1,y+length+7,width-2,1);
	line(x+2,y+length+9,width-4,1);
	/*进度条中间可变动的线*/
	for(uint8_t x1=x+3;x1<x+width-3;x1++)
	{
		 uint8_t change;
		 change=value-1;
	   line(x1,y+3,value,2);
		
	}
}
/**
 * @function: void progress_down(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
 * @description: 向上进度条，左下角起始坐标(0,0)
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} length 中间可变条长度0~128
 * @param {uint8_t} width  0~64
 * @param {uint8_t} value  变量，最大值要与length相等
 * @return {*}
 */
void progress_down(uint8_t  x,uint8_t y,uint8_t length,uint8_t width,uint8_t value)
{
	uint8_t n=x+5;
	/*进度条两个边框*/
	Draw_square(x,y,width,length+5);
	Draw_square(x+1,y+1,width-2,length+3);
	/*进度条两边的四条竖线*/	
	line(x+1,y-2,width-2,1);
	line(x+2,y-4,width-4,1);
	line(x+1,y+length+7,width-2,1);
	line(x+2,y+length+9,width-4,1);
	/*进度条中间可变动的线*/
	for(uint8_t x1=x+3;x1<x+width-3;x1++)
	{
		 uint8_t change;
		 change=value-1;
	   line(x1,y+length+2,value,4);
		
	}
}
/**
 * @function:      void pro_up(u8 x,u8 y,u8 length,u8 value)
 * @description:   向下选项条，左下角起始坐标(0,0)
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} length 
 * @param {uint8_t} value  选项值变量，
 * @return {*}
 */
uint8_t y_move=0;
uint8_t y_move_1=0;
void pro_up(u8 x,u8 y,u8 length,u8 value)
{
//	长度：7X2+(横线个数-1)X6+横线个数
	uint8_t i,j;
	if(value>=5)//使横线上下移动
	{
		if(y_move_1<(value-5)*6)
		y_move_1+=2;
		if(y_move_1>(value-5)*6)
		y_move_1-=2;		
	}
	line(x,y,length,4);//向下,长直线
	line(x-1,y+y_move_1,3,4);//向下
	line(x+1,y+y_move_1,3,4);//向下	
	if(value>=6)//最后一个横线的值(0~6)
	{
		line(x-1,y-length+1,3,2);//向上
		line(x+1,y-length+1,3,2);//向上
	}	
	for(i=1;i<=7;i++)//7个横线
	{
		 line(x-2,y-6*i+y_move_1,5,1);//向右	，短横线	
	}
	if(y_move<value*6)//使粗横线上下移动
		y_move+=2;
	if(y_move>value*6)
		y_move-=2;
  if(value>=6)//到底了保持不动
	{	
   		y_move=5*6;
  }
	for(j=x-3;j<=x+3;j++)
	{
	line(j,y-4-y_move,3,4);	
	}
	
}



/**
 * @function:       void OLED_flash_8_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8])
 * @description:    将8*8 ASCII字符集点阵数组元素放入Grame[8][128]中
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} A[][8] 8*8数组
 * @return {*}
 */

void OLED_flash_8_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8])
{
	uint8_t i = 0;
	uint8_t j = 0;
	for(i = 0;i < 8;i++)
	{
		Grame[y][x++] = A[n][j++];
	}
}

/**
 * @function:       uint8_t OLED_flash_Show_Char_8(uint8_t x,uint8_t y,uint8_t C)
 * @description:    显示8*8字符
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} C  字符，用单引号括住
 * @return {*}
 */
uint8_t OLED_flash_Show_Char_8(uint8_t x,uint8_t y,uint8_t C)
{
	uint8_t n;
	if(C < 32 || C > 126)
	{
		return 0;
	}
	n = C - 32;
	OLED_flash_8_Fast(x,y,n,Char_8);
	
	return 1;
}
/**
 * @function:       void OLED_flash_String_8(uint8_t x,uint8_t y,char *C)
 * @description:    显示8*8字符串
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {char}    *C 字符串，用双引号括住
 * @return {*}
 */

void OLED_flash_String_8(uint8_t x,uint8_t y,char *C)
{
	while(*C != '\0')
	{
		if(y > 7)
		{
			break;
		}
		if(x > 120)
		{
			x = 0;
			y = y + 1;
		}
		OLED_flash_Show_Char_8(x,y,*C);
		x = x + 8;
		C++;
	}
}
/**
 * @function:       void OLED_flash_6_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8])
 * @description:    将8*6 ASCII字符集点阵数组元素放入Grame[8][128]中
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} A[][6] 8*6数组
 * @return {*}
 */

void OLED_flash_6_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][6])
{
	uint8_t i = 0;
	uint8_t j = 0;
	for(i = 0;i < 6;i++)
	{
		Grame[y][x++] = A[n][j++];
	}
}
/**
 * @function:       uint8_t OLED_flash_Show_Char_8(uint8_t x,uint8_t y,uint8_t C)
 * @description:    显示8*6字符
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t} C  字符，用单引号括住
 * @return {*}
 */

uint8_t OLED_flash_Show_Char_6(uint8_t x,uint8_t y,uint8_t C)
{
	uint8_t n;
	if(C < 32 || C > 126)
	{
		return 0;
	}
	n = C - 32;
	OLED_flash_6_Fast(x,y,n,F6x8);
	
	return 1;
}
/**
 * @function:       void OLED_flash_String_8(uint8_t x,uint8_t y,char *C)
 * @description:    显示8*6字符串
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {char}    *C 字符串，用双引号括住
 * @return {*}
 */
void OLED_flash_String_6(uint8_t x,uint8_t y,char *C)
{
	while(*C != '\0')
	{
		if(y > 7)
		{
			break;
		}
		if(x > 120)
		{
			x = 0;
			y = y + 1;
		}
		OLED_flash_Show_Char_6(x,y,*C);
		x = x + 8;
		C++;
	}
}
/**
 * @function:       void left_or_right_Throttle_fast(uint8_t x,uint8_t y,uint16_t variable)
 * @description:    显示左右油门,左上角原点
 * @param {uint8_t}  x  0~87
 * @param {uint8_t}  y  0~3
 * @param {uint16_t} variable 摇杆Y轴变量
 * @return {*}
 */

void left_or_right_Throttle_fast(uint8_t x,uint8_t y,uint16_t variable)
{
    if(variable<=4095&&variable>3640)
		{
			OLED_40x40BMP_fast(x,y,6,0,BMP40x40);//油门-4	 
			OLED_ShowNumber_Fast(18+x,y*8+40, -4,1,12,0);	
		}
		else if (variable<=3640&&variable>3185)
		{
			OLED_40x40BMP_fast(x,y,7,0,BMP40x40);	//油门-3
			OLED_ShowNumber_Fast(18+x,y*8+40, -3,1,12,0);	
		}
		else if (variable<=3185&&variable>2730)
		{
			OLED_40x40BMP_fast(x,y,8,0,BMP40x40);	//油门-2
			OLED_ShowNumber_Fast(18+x,y*8+40, -2,1,12,0);	
		}
		else if (variable<=2730&&variable>2275)
		{
			OLED_40x40BMP_fast(x,y,9,0,BMP40x40);	//油门-1
			OLED_ShowNumber_Fast(18+x,y*8+40, -1,1,12,0);	
		}
		else if (variable<=2275&&variable>1820)
		{
			OLED_40x40BMP_fast(x,y,10,0,BMP40x40);	//油门 0
			OLED_ShowNumber_Fast(18+x,y*8+40, 0,1,12,0);	
		}
		else if (variable<=1820&&variable>1365)
		{
			OLED_40x40BMP_fast(x,y,11,0,BMP40x40);	//油门 1
			OLED_ShowNumber_Fast(18+x,y*8+40, 1,1,12,0);	
		}
		else if (variable<=1365&&variable>910)
		{
			OLED_40x40BMP_fast(x,y,12,0,BMP40x40);	//油门 2
			OLED_ShowNumber_Fast(18+x,y*8+40, 2,1,12,0);	
		}
		else if (variable<=910&&variable>455)
		{
			OLED_40x40BMP_fast(x,y,13,0,BMP40x40);	//油门 3
			OLED_ShowNumber_Fast(18+x,y*8+40, 3,1,12,0);	
		}
		else if (variable<=455&&variable>=0)
		{
			OLED_40x40BMP_fast(x,y,14,0,BMP40x40);	//油门 4 
			OLED_ShowNumber_Fast(18+x,y*8+40, 4,1,12,0);	
		}			
}
/**
 * @function:       void Slide_point(uint8_t x,uint8_t y,uint8_t length,uint8_t width,uint16_t var_max,uint16_t variable)
 * @description:    显示框内滑动点,左下角原点
 * @param {uint8_t}  x        0~127,Draw_square中的x
 * @param {uint8_t}  y        0~63,Draw_square中的y
 * @param {uint8_t}  length   Draw_square中的长度，实际长度比这个值大一
 * @param {uint8_t}  width    Draw_square中的宽度，实际宽度比这个值大一
 * @param {uint16_t} var_max  变量最大值，例如摇杆x轴变量最大值4095
 * @param {uint16_t} variable 例如摇杆x轴变量
 * @return {*}
 */

void Slide_point(uint8_t x,uint8_t y,uint8_t length,uint8_t width,uint16_t var_max,uint16_t variable)
{
	  Draw_square(x,y,length,width);
		if(width%2==0)//中间横条为奇数条
		{			
			for(uint8_t i=1;i<=width/2;i++)//画width/2条直线
			{
			 if(i==width/2)	
	       line(variable*(length-width)/var_max+(x+1),y+width/2,width-1,1);//最中间那条
			 else
			 {
				 //以中间为对称轴两边对称的两条
	       line(variable*(length-width)/var_max+(x+1+i),y+width/2-i,width-1-i*2,1);//下面那条
	       line(variable*(length-width)/var_max+(x+1+i),y+width/2+i,width-1-i*2,1);//上面那条
				 
			 }
			}
		}
		else//中间横条为偶数条
		{
			for(uint8_t i=0;i<(width-1)/2;i++)
			{					
	      line(variable*(length-width)/var_max+(x+1+i),y+(width-1)/2-i,width-1-2*i,1);//两条对称线中的下面那一条
	      line(variable*(length-width)/var_max+(x+1+i),y+(width-1)/2+1+i,width-1-2*i,1);//两条对称线中的上面那一条			
			}	
		}	
	}
/**
 * @function:       void progress_bar(uint8_t x,uint8_t y,uint8_t length,uint8_t width,uint16_t variable)
 * @description:    显示进度条,左下角原点
 * @param {uint8_t}  x        0~127,Draw_square中的x
 * @param {uint8_t}  y        0~63,Draw_square中的y
 * @param {uint8_t}  length   Draw_square中的长度，实际长度比这个值大一
 * @param {uint8_t}  width    Draw_square中的宽度，实际宽度比这个值大一
 * @param {uint16_t} variable 例如拨码电位器变量，该变量最大值要比length大2或3
 * @return {*}
 */

void progress_bar(uint8_t x,uint8_t y,uint8_t length,uint8_t width,uint16_t variable)
{
	Draw_square(x,y,width,length);//从上到下为宽，向上进度条length和width要互换位置
	for (uint8_t i=1;i<width-3;i++)
	{
	  line(x+1+i,y+2,variable,2);	
	}	
}
/**
 * @function:       void rocher_position(long x,long y,uint8_t side,uint8_t variable_x,uint8_t variable_y)
 * @description:    显示摇杆框内可移动点,左下角原点
 * @param {long}     x          0~127,Draw_square中的x
 * @param {long}     y          0~63,Draw_square中的y
 * @param {uint8_t}  side       正方形边长
 * @param {uint8_t}  variable_x 摇杆x轴的值,最大值不得超过side
 * @param {uint16_t} variable_y 摇杆y轴的值,最大值不得超过side
 * @return {*}
 */

//如果x小于0，则要用long类型
void rocher_position(long x,long y,uint8_t side,uint8_t variable_x,uint8_t variable_y)
{
	Draw_square(x,y,side,side);
	line(x,y+side/2,side,5);//向右虚线
	line(x+side/2,y,side,6);//向上虚线
	
	for(u8 i=0;i<=1;i++)
	{
  	line(x+1+i+variable_x,y+3+i+variable_y,4-2*i,1);
  	line(x+1+i+variable_x,y+2-i+variable_y,4-2*i,1);		
	}
	
}
/**
 * @function:       void propulsion_strip(long x,u8 y,u8 select)
 * @description:    显示箭头推进,左上角原点
 * @param {long}     x          0~127
 * @param {u8}       y          0~63
 * @param {uint8_t}  select     切换中间显示的内容
 * @return {*}
 */

 uint8_t progress_move;//此处在遥控器代码要加extern,在其他调用则去掉
void propulsion_strip(long x,u8 y,u8 select)
{
  if(progress_move<=24)
	{
		if(progress_move==24)
			progress_move=0;
		
		if(progress_move>2&&progress_move<=24)
		{
         OLED_ShowChar_Fast(x,y,'>',16,0);
         OLED_ShowChar_Fast(x+108,y,'<',16,0);		
		}
        if(progress_move>9&&progress_move<=24)
		{
         OLED_ShowChar_Fast(x+10,y,'>',16,0);
         OLED_ShowChar_Fast(x+98,y,'<',16,0);
		}
        if(progress_move>16&&progress_move<=24)
		{
			OLED_ShowChar_Fast(x+20,y,'>',16,0);
			OLED_ShowChar_Fast(x+88,y,'<',16,0);
		}			
     if (progress_move>=0&&progress_move<=2)
		{
      OLED_ShowChar_Fast(x,y,' ',16,0);
      OLED_ShowChar_Fast(x+88,y,' ',16,0);		
      OLED_ShowChar_Fast(x+10,y,' ',16,0);
      OLED_ShowChar_Fast(x+98,y,' ',16,0);			
      OLED_ShowChar_Fast(x+20,y,' ',16,0);
      OLED_ShowChar_Fast(x+108,y,' ',16,0);
		}			
	  progress_move++;	
	}
	if(select==0)
   OLED_ShowString_Fast(x+43,y,"GAME",16,0,3);
	else if(select==1)
   OLED_ShowString_Fast(x+35,y,"ROCHER",16,0,3);
}
/**
 * @function:       void draw_wave(u8 x,u8 x1,u8 width_x,u8 y,u8 hight_y,uint16_t variable)
 * @description:    显示波形图,左下角原点
 * @param {u8}     x          波形的最左端的横坐标，0~124
 * @param {u8}     x1         波形的最右端的横坐标，0~124
 * @param {u8}     width_x    坐标系横坐标宽度，0~127
 * @param {u8}       y        波形的最右端的纵坐标，0~64
 * @param {uint8_t} hight_y   坐标系纵坐标宽度，3~61
 * @return {*}
 */

u8 xtime;
void draw_wave(u8 x,u8 x1,u8 width_x,u8 y,u8 hight_y,uint16_t variable)
{
	for(xtime=x;xtime<x1;xtime++)
		waveform[xtime]=waveform[xtime+1];
	
//	waveform[127]=Left_dial*50/4095+5;
	waveform[x1]=variable+y;
	line(x1,y,width_x,3);//向左
	line(x1,y,hight_y,2);//向上
  for(uint8_t i=1;i<=2;i++)
	{
		line(x1-width_x+5,y+i,5-2*i,3);//x轴箭头
		line(x1-width_x+5,y-i,5-2*i,3);
		
		line(x1+i,y+hight_y-5,5-2*i,2);//y轴箭头
		line(x1-i,y+hight_y-5,5-2*i,2);	
	}
	OLED_ShowChar_Fast(x1-width_x+2,63-(y+14),'x',12,0);
	OLED_ShowChar_Fast(x1-10,63-(y+hight_y+0),'y',12,0);
	for(xtime=x;xtime<x1;xtime+=1)
	{	
	   OLED_DrawLine(xtime,waveform[xtime],xtime+1,waveform[xtime+1]);		
	}	
	
}
/***************************多级菜单里的函数***************************/
/**
 * @function:       void OLED_DrawPoint(int x,int y,uint8_t t)
 * @description:    画点，左上角为原点
 * @param {uint8_t} x  0~127
 * @param {uint8_t} y  0~63
 * @param {uint8_t}    t 0 填充 1 清空
 * @return {*}
 */
void OLED_DrawPoint(int x,int y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x<0 || x>127 || y<0 || y>63)return;//
	pos=y/8;//左上角为原点	
	bx=y%8;
	temp=1<<bx;//左上角为原点
	if(t)Grame[pos][x]&=~temp;
	else Grame[pos][x]|=temp;
//	pos=7-y/8;//左下角为原点	
//	temp=1<<(7-bx);//左下角为原点	
//	if(t)Grame[pos][x]|=temp;
//	else Grame[pos][x]&=~temp;
	
}

/**
 * @function:       void OLED_ShowCH_Fast(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
 * @description:    显示一半汉字，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {uint8_t} chr   数组元素的序号
 * @param {uint8_t} size  字体大小:12/16 12：12*12的汉字，16: 16*16的汉字
 * @param {uint8_t} mode  1 反白显示 0 正常显示
 * @return {*}
 */
void OLED_ShowCH_Fast(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint8_t y0=y;
	  
  for(t=0;t<size;t++)
  {
		if(size==16)
			temp=oled_CH_1616[chr][t];		 //调用中文字库
		else
			temp=oled_CH_1212[chr][t];
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
  }          
}
/**
 * @function:       void OLED_Show_CH(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
 * @description:    显示一个汉字，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {uint8_t} chr   数组元素的序号
 * @param {uint8_t} size  字体大小:12/16 12：12*12的汉字，16: 16*16的汉字
 * @param {uint8_t} mode  1 反白显示 0 正常显示
 * @return {*}
 */
void OLED_Show_CH_Fast(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
	OLED_ShowCH_Fast(x,y,chr*2,size,mode);OLED_ShowCH_Fast(x+size/2,y,chr*2+1,size,mode);
}

/**
 * @function:       uint8_t OLED_findoneCN(uint8_t ch1,uint8_t ch2,uint8_t size)
 * @description:    在字库里找字
 * @param {uint8_t} ch1   低位
 * @param {uint8_t} ch2   高位
 * @param {uint8_t} size  字体大小:12/16 12：12*12的汉字，16: 16*16的汉字
 * @return {*}
 */
uint8_t OLED_findoneCN_Fast(uint8_t ch1,uint8_t ch2,uint8_t size)
{
	uint8_t j=0;
	if(size==12)
	{
		while(character12[j]!='\0')
		{
			if(ch1==character12[j]&&ch2==character12[j+1])
				return j/2+1;
			j+=2;
		}
	}
	else
		while(character16[j]!='\0')
		{
			if(ch1==character16[j]&&ch2==character16[j+1])
				return j/2+1;
			j+=2;
		}
		
	return 0;
}
/**
 * @function:       void OLED_Show_chString(uint8_t x, uint8_t y,uint8_t ch[],uint8_t size)
 * @description:    显示一串汉字，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {uint8_t} ch[]  数组
 * @param {uint8_t} size  字体大小:12/16 12：12*12的汉字，16: 16*16的汉字
 * @return {*}
 */
void OLED_Show_chString_Fast(uint8_t x, uint8_t y,uint8_t ch[],uint8_t size)
{
	int j=0,k;	
	while(ch[j] != '\0')
	{
		if(ch[j]>0x80)//汉字
		{
			k=OLED_findoneCN_Fast(ch[j],ch[j+1],size);
			if(k!=0)OLED_Show_CH_Fast(j*size/2+x,y,k-1,size,1);
			j+=2;		
		}
		else//ASCLL
		{
			OLED_ShowChar_Fast(j*size/2+x,y,ch[j],size,1);
			j++;
		}		
	}
}
/**
 * @function:       void OLED_ShowChar_Fast(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
 * @description:    在指定位置显示一个字符，包括部分字符，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {uint8_t} chr   字符，单引号括住
 * @param {uint8_t} size  字体大小:12/16 12：8*12的字符，16: 8*16的字符
 * @param {uint8_t} mode  1 反白显示 0 正常显示
 * @return {*}
 */
void OLED_ShowChar_Fast(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	chr=chr-' ';//得到偏移后的值   
  for(t=0;t<size;t++)
  {
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体	                          
    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80) OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
  }
}
/**
 * @function:      void OLED_ShowString_Fast(uint8_t x,uint8_t y,char *p,uint8_t size,uint8_t mode)
 * @description:    在指定位置显示字符串，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {char}    *p    字符串起始地址
 * @param {uint8_t} size  字体大小:12/16 12：8*12的字符，16: 8*16的字符
 * @param {uint8_t} mode  1 反白显示 0 正常显示
 * @param {uint8_t} line_variable  0 有超出屏幕自动换行功能  1  x>127时，自动换行
                                 //2 y>63时，换到第一行      3  没有自动换行功能，可用于字符移动出屏幕时
 * @return {*}
 */

void OLED_ShowString_Fast(uint16_t x,uint8_t y,char *p,uint8_t size,uint8_t mode,uint8_t line_variable)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
  while(*p!='\0')
  {  
		if(line_variable==0)
		{
			if(x>MAX_CHAR_POSX){x=0;y+=16;}//换行		
			if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}//最后一行超出屏幕的字符移到第一行
      OLED_ShowChar_Fast(x,y,*p,size,mode);	   
		}
		else if(line_variable==1)
		{
			if(x>MAX_CHAR_POSX){x=0;y+=16;}//换行					
      OLED_ShowChar_Fast(x,y,*p,size,mode);	 
		}	
		else if(line_variable==2)
		{
			if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}//最后一行超出屏幕的字符移到第一行
      OLED_ShowChar_Fast(x,y,*p,size,mode);	 
		}	
		else if(line_variable==3)
		{
      OLED_ShowChar_Fast(x,y,*p,size,mode);	 
		}	
		
		
    x+=8;
    p++;
  }  
}

/**
 * @function:      void OLED_ShowNumber_Fast(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
 * @description:    在指定位置显示正负数字，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {int}     num   数值-2.1*109^9~2.1*10^9   int(-32768~32767)或0~65535(无符号)
 * @param {uint8_t} len   数字长度(位数)
 * @param {uint8_t} size  字体大小:12/16 12：6*8的数字，16: 8*16的数字  ,宽是8，高是16
 * @param {uint8_t} mode  1 反白显示 0 正常显示
 * @return {*}
 */
void OLED_ShowNumber_Fast(uint8_t x,uint8_t y,int num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp,i=0;//i为负数标志位
	uint8_t enshow=0;
  if(num<0)//如果为负数，
	{
//		len+=1;//将字长加一
		i=1;
		num=-num;					
	}
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar_Fast(x+(size/2)*t,y,' ',size,mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar_Fast(x+(size/2)*t,y,temp+'0',size,mode); 
	}
	if(i==1)//如果为负数，在最前面加'-'
	{
	 	OLED_ShowChar_Fast(x-8,y,'-',size,mode);
		i=0; 		
	}
}
/**
 * @function:      void OLED_ShowdecimalNumber_Fast(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size,uint8_t mode)
 * @description:    在指定位置显示正负小数数字，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {float}   num   数值大小
 * @param {uint8_t} z_len 整数数字长度(位数)
 * @param {uint8_t} f_len 小数数字长度(位数)
 * @param {uint8_t} size  字体大小:12/16 12：6*8的数字，16: 8*16的数字
 * @param {uint8_t} mode  1 反白显示 0 正常显示
 * @return {*}
 */

void OLED_ShowdecimalNumber_Fast(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size,uint8_t mode)
{
	uint8_t t,temp,flag=0,i=0;//i为负数标志位
	uint8_t enshow=0;
	int z_temp,f_temp;      
	z_temp = num;
	//整数部分
	if(num<0)//如果为负数，
	{
//    z_len+=1;//将字长加一
		i=1;
		num=-num;				
	}
	for(t=0;t<z_len;t++)
	{
		z_temp =abs(z_temp);
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar_Fast(x+t*8,y,' ',size,mode);
				continue;
			}
			else
			  enshow=1;
		}
		if(num < 0 && flag == 0)
			{OLED_ShowChar_Fast(x+(t-1)*8,y,'-',size,mode);flag=1;}
		//if(fabs(num) < oled_pow(10,z_len-1))
			//OLED_ShowChar((t-1)*8,y,' ',16,1);
		OLED_ShowChar_Fast(x+t*8,y,temp+'0',size,mode); 
	}
	//小数点
	OLED_ShowChar_Fast(x+z_len*8,y,'.',size,mode); 
	num =fabs(num);
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar_Fast(x+(t+z_len+1)*8,y,temp+'0',size,mode); 
	}
	if(i==1)//如果为负，就将最前的一位赋值‘-’
	{
		OLED_ShowChar_Fast(x-8,y,'-',size, mode);
		i=0;
	}

}
/**
 * @function:      void ellipse(int x,int y,uint8_t major_semi_axis,uint8_t minor_semi_axis,float t)
 * @description:    在指定位置显示空心椭圆，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {float}   major_semi_axis   长半轴
 * @param {uint8_t} minor_semi_axis   短半轴
 * @param {float}   t     顺时针旋转角度
 * @return {*}
 */
void ellipse(int x,int y,uint8_t major_semi_axis,uint8_t minor_semi_axis,float t)
{
	int x1,y1;
	uint8_t a=major_semi_axis,b=minor_semi_axis;
	float c;
	t=t*0.01745f;
	for(c=0.f;c<2*3.14f;c+=0.04f)
	{
		x1=x+a*cos(c)*cos(t)-b*sin(c)*sin(t);
		y1=y+b*sin(c)*cos(t)+a*cos(c)*sin(t);
		OLED_DrawPoint(x1,y1,0);
	}
}
/**
 * @function:      void OLED_invert(int x,int y)
 * @description:   像素反色，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @return {*}
 */
void OLED_invert(int x,int y)
{
	u8 pos,bx,temp=0;
	if(x<0 || x>127 || y<0 || y>63)return;//超出范围了
//	pos=7-y/8;
//	temp=1<<(7-bx);
	pos=y/8;
	bx=y%8;
	temp=1<<(bx);
	
	if((Grame[pos][x] & temp) == temp)
		Grame[pos][x]&=~temp;
	else
		Grame[pos][x]|=temp;
		
}
/**
 * @function:      void OLED_invert_line(u8 x,u8 y,u8 length,u8 mode)
 * @description:    在指定位置显示反色线，左上角为原点
 * @param {uint8_t} x     0~127
 * @param {uint8_t} y     0~63
 * @param {float}   length   反色线长度，从上到下，从左到右伸长
 * @param {uint8_t} mode     模式 ：1 向右 0向下
 * @return {*}
 */
void OLED_invert_line(u8 x,u8 y,u8 length,u8 mode)
{
	if(mode == 1)
		for(u8 x1=x;x1<x+length;x1++)
			OLED_invert(x1,y);
	else
		for(u8 y1=y;y1<y+length;y1++)
			OLED_invert(x,y1);
}
/**
 * @function:      void OLED_invert_full(u8 x,u8 y,u8 width,u8 length)
 * @description:    在指定位置显示反色区域，左上角为原点
 * @param {uint8_t} x      0~127
 * @param {uint8_t} y      0~63
 * @param {uint8_t} width  反白区域宽度
 * @param {float}   length   反色线长度，从上到下，从左到右伸长
 * @return {*}
 */

void OLED_invert_full(u8 x,u8 y,u8 width,u8 length)
{
	for(u8 y1=y;y1<y+width;y1++)//纵向宽度为16
		OLED_invert_line(x,y1,length,1);
}
/**
 * @function:      void OLED_sel(float x,float y,float len)
 * @description:    在指定位置显示反色区域，左上角为原点
 * @param {float} x     0~127
 * @param {float} y     0~63
 * @param {float} len   反色线长度，从上到下，从左到右伸长,1个单位代表半个16*16汉字字长
 * @return {*}
 */

float prex=10,prey=1,prelen=1;
void OLED_sel(float x,float y,float len)
{
	float p=0.66f,x_err,y_err,len_err,len1=8*len+4;//改变p,可改变速度
	if(y>=1 && y<=3) y=16*(y-1)+16.5f;//y每加减一，纵向移动16
	OLED_ShowChar_Fast(0,prey,' ',16,0);
	OLED_invert_full(prex,prey,16,prelen);
	x_err=x-prex;
	y_err=y-prey;
	prex=prex+p*x_err;
	prey=prey+p*y_err;
	len_err=len1-prelen;
	prelen=prelen+p*len_err;
	OLED_ShowChar_Fast(0,prey,'>',16,0);
	OLED_invert_full(prex,prey,16,prelen);
}
/**
 * @function:      void OLED_sel_time(float x,float y,float len)
 * @description:    在指定位置显示修改时间界面反色区域，左上角为原点
 * @param {float} x     0~127
 * @param {float} y     0~63
 * @param {float} len   反色线长度，从上到下，从左到右伸长,1个单位代表半个16*16汉字字长
 * @return {*}
 */

float prex_time=0,prey_time=0,prelen_time=1;
void OLED_sel_time(float x,float y,float len)
{
	float p=0.56f,x_err,y_err,len_err,len1=8*len+4;//改变p,可改变速度
	if(y>=0 && y<=2) y=16*y+0.5f;//y每加减一，纵向移动16
	if(x>=0 && x<=1) x=67*x+0.5f;//x每加减一，横向移动67
	
	OLED_invert_full(prex_time,prey_time,16,prelen_time);
	x_err=x-prex_time;
	y_err=y-prey_time;
	prex_time=prex_time+p*x_err;
	prey_time=prey_time+p*y_err;
	len_err=len1-prelen_time;
	prelen_time=prelen_time+p*len_err;
	OLED_invert_full(prex_time,prey_time,16,prelen_time);
}
/**
 * @function:      void OLED_sel_wave(float x,float y,float len)
 * @description:    在指定位置显示波形图界面反色区域，左上角为原点
 * @param {float} x     0~127
 * @param {float} y     0~63
 * @param {float} len   反色线长度，从上到下，从左到右伸长,1个单位代表半个16*16汉字字长
 * @return {*}
 */

float prex_wave=0,prey_wave=11,prelen_wave=2;
void OLED_sel_wave(float x,float y,float len)
{
	float p=0.56f,x_err,y_err,len_err,len1=8*len+4;//改变p,可改变速度
	if(y>=1 && y<=3) y=21*(y-1)+11.5f;//y每加减一，纵向移动16
//	if(x>=0 && x<=1) x=67*x+0.5f;//x每加减一，横向移动67
	
	OLED_invert_full(prex_wave,prey_wave,10,prelen_wave);
	x_err=x-prex_wave;
	y_err=y-prey_wave;
	prex_wave=prex_wave+p*x_err;
	prey_wave=prey_wave+p*y_err;
	len_err=len1-prelen_wave;
	prelen_wave=prelen_wave+p*len_err;
	OLED_invert_full(prex_wave,prey_wave,10,prelen_wave);
}
/**
 * @function:      void OLED_sel_avoid(float x,float y,float len)
 * @description:    在指定位置显示波形图界面反色区域，左上角为原点
 * @param {float} x     0~127
 * @param {float} y     0~63
 * @param {float} len   反色线长度，从上到下，从左到右伸长,1个单位代表半个16*16汉字字长
 * @return {*}
 */

float prex_avoid=0,prey_avoid=17,prelen_avoid=2;
void OLED_sel_avoid(float x,float y,float len)
{
	float p=0.56f,x_err,y_err,len_err,len1=8*len+4;//改变p,可改变速度
	if(y>=1 && y<=4) y=16*(y-1)+16.5f;//y每加减一，纵向移动16
	if(x>=0 && x<=1) x=84*x+0.5f;//x每加减一，横向移动67
	
	OLED_invert_full(prex_avoid,prey_avoid,14,prelen_avoid);
	x_err=x-prex_avoid;
	y_err=y-prey_avoid;
	prex_avoid=prex_avoid+p*x_err;
	prey_avoid=prey_avoid+p*y_err;
	len_err=len1-prelen_avoid;
	prelen_avoid=prelen_avoid+p*len_err;
	OLED_invert_full(prex_avoid,prey_avoid,14,prelen_avoid);
}
/**
 * @function:      void OLED_sel_follow(float x,float y,float len)
 * @description:    在指定位置显示波形图界面反色区域，左上角为原点
 * @param {float} x     0~127
 * @param {float} y     0~63
 * @param {float} len   反色线长度，从上到下，从左到右伸长,1个单位代表半个16*16汉字字长
 * @return {*}
 */

float prex_follow=0,prey_follow=17,prelen_follow=2;
void OLED_sel_follow(float x,float y,float len)
{
	float p=0.56f,x_err,y_err,len_err,len1=8*len+4;//改变p,可改变速度
	if(y>=1 && y<=4) y=16*(y-1)+16.5f;//y每加减一，纵向移动16
	if(x>=0 && x<=1) x=84*x+0.5f;//x每加减一，横向移动67
	
	OLED_invert_full(prex_follow,prey_follow,14,prelen_follow);
	x_err=x-prex_follow;
	y_err=y-prey_follow;
	prex_follow=prex_follow+p*x_err;
	prey_follow=prey_follow+p*y_err;
	len_err=len1-prelen_follow;
	prelen_follow=prelen_follow+p*len_err;
	OLED_invert_full(prex_follow,prey_follow,14,prelen_follow);
}



/**
 * @function:      void OLED_BMP_slide(float x,float y)
 * @description:    丝滑移动40x40BMP图片
 * @param {float} x     0~127
 * @param {float} y     0~63
 * @param {float} len   反色线长度，从上到下，从左到右伸长
 * @return {*}
 */
float present_x=0,present_y=1;
void OLED_BMP_slide(float x,float y,uint8_t select)
{
	float p=0.40f,x_err,y_err,len_err;
	if(x>=1 && x<=127)x=1*(x-1)+16.5f;//16.5说明四舍五入
	OLED_40x40BMP_fast(present_x,present_y,5,0,BMP40x40);//消残影
	x_err=x-present_x;
	y_err=y-present_y;
	present_x=present_x+p*x_err;
	present_y=present_y+p*y_err;
	OLED_40x40BMP_fast(present_x,present_y,select,0,BMP40x40);
	
}
/**
 * @function:      uint16_t OLED_slide(float m,uint8_t move_num)
 * @description:    40x40图片每次丝滑移动move_num格
 * @param {float}   m     0,1,2,3,4...
 * @param {uint8_t} move_num 0~255
 * @param {float} len   反色线长度，从上到下，从左到右伸长
 * @return {*}
 */
float present_m=0;
uint16_t OLED_slide(float m,uint8_t move_num)
{
	float p=0.10f,m_err;//调节p，可调速度
	m=move_num*m+0.5f;//四舍五入
	m_err=m-present_m;
	present_m=present_m+p*m_err;
  return present_m;//当前移动后的值
}
/**
 * @function:      uint16_t OLED_slide(float m,uint8_t move_num)
* @description:    选项名称每次丝滑移动move_num格
 * @param {float}   m     0,1,2,3,4...
 * @param {uint8_t} move_num 0~255
 * @param {float} len   反色线长度，从上到下，从左到右伸长
 * @return {*}
 */
float present_m_name=0;
uint16_t OLED_slide_name(float m,uint8_t move_num)
{
	float p=0.10f,m_err;//调节p，可调速度
	m=move_num*m+0.5f;//四舍五入
	m_err=m-present_m_name;
	present_m_name=present_m_name+p*m_err;
  return present_m_name;//当前移动后的值
}
/**
 * @function:      uint16_t OLED_slide_light(float m,uint8_t move_num)
* @description:    亮度条每次丝滑移动move_num格
 * @param {float}   m     0,1,2,3,4...
 * @param {uint8_t} move_num 0~255
 * @param {float} len   反色线长度，从上到下，从左到右伸长
 * @return {*}
 */
float present_m_light=0;
uint16_t OLED_slide_light(float m,uint8_t move_num)
{
	float p=0.50f,m_err;//调节p，可调速度
	m=move_num*m+0.5f;//四舍五入
	m_err=m-present_m_light;
	present_m_light=present_m_light+p*m_err;
  return present_m_light;//当前移动后的值
}
/**
 * @function:      uint16_t OLED_slidenum_light(float m,uint8_t move_num)
* @description:    亮度条数字每次丝滑增加move_num
 * @param {float}   m     0,1,2,3,4...
 * @param {uint8_t} move_num 0~255
 * @param {float} len   反色线长度，从上到下，从左到右伸长
 * @return {*}
 */
float present_num_light=0;
uint16_t OLED_slidenum_light(float m,uint8_t move_num)
{
	float p=0.50f,m_err;//调节p，可调速度
	m=move_num*m+0.5f;//四舍五入
	m_err=m-present_num_light;
	present_num_light=present_num_light+p*m_err;
  return present_num_light;//当前移动后的值
}

/********************************************************/
/**
 * @function: void OLED_Refresh(uint8_t mod)
 * @description: 在OLED全屏刷新缓存区Grame[8][128]
 * @return {*}
 */

void OLED_Refresh(uint8_t mod)
{
	uint8_t y,x;
	uint8_t buf[3];
	for(y=0;y<8;y++)
	{
		buf[0]=0xb0+y;
		buf[1]=0x00;
		buf[2]=0x10;
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,buf,3,0x100);
//		OLED_WR_CMD(0xb0+y);    //设置页地址（0~7）
//		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
//		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
//		OLED_WR_DATA(0x78);
//		OLED_WR_DATA(0);
		for(x=0;x<128;x++)
    {		
			if(mod==1)
			  OLED_WR_DATA(~Grame[y][x]);
	    else
				OLED_WR_DATA(Grame[y][x]);
    }
	}
}
/********************************************************/
/**
 * @function: void  OLED_40x40_fast(uint8_t x,char y,uint8_t MOD ,uint8_t A[] )
 * @description: 在OLED特定区域显示40x40一维数组图片
 * @param {uint8_t} x  0~127
 * @param {char}    y  0~7
 * @param {uint8_t} A[]待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */

void  OLED_40x40BMP_fast(long x,char y,uint8_t select,uint8_t Color_Turn ,uint8_t A[][40] )//数组赋值,40x40图片
{//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;	
	uint8_t a = 0;	
	
//	uint8_t x_bar = 0;	
//	char x_bar=x;
	long x_bar=x;
	
	for(i=0;i<5;i++)
	{
		x=x_bar;//每行从该x位置起始
		k=0;//重新计数 0->40
		for(j=0;j<40;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		        Grame[y][x] = ~A[5*select+i][k];
        else 
				   Grame[y][x] =A[5*select+i][k];  
			}				
			x++;
			k++;//放在外面			
		}
		y++;
	}
}
/**
 * @function: void  OLED8x8BMP_fast(uint8_t x,char y,uint8_t Color_Turn ,uint8_t select ,uint8_t A[][16] )
 * @description: 在OLED特定区域显示8x8二维数组图片
 * @param {uint8_t} x  0~127
 * @param {char}    y  0~7
 * @param {uint8_t} A[][8]待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @param {uint8_t} select二维数组的第几元素
 * @return {*}
 */

void  OLED8x8BMP_fast(uint8_t x,char y,uint8_t select ,uint8_t Color_Turn ,uint8_t A[][8] )//数组赋值,40x40图片
{//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;	
	uint8_t m = 0;
	
	char x_bar=x;
	x=x_bar;//每行从该x位置起始
	for(j=0;j<8;j++)
	{  
		if(x >= 0 && x <= 127)//只在屏幕内显示
		{ 
			if(Color_Turn)
					Grame[y][x] = ~A[select][k];
			else 
				 Grame[y][x] =A[select][k];  
		}				
		x++;
		k++;//放在外面
	}
}

/**
 * @function: void  OLED16x16BMP_fast(uint8_t x,char y,uint8_t Color_Turn ,uint8_t select ,uint8_t A[][16] )
 * @description: 在OLED特定区域显示16x16二维数组图片
 * @param {uint8_t} x  0~127
 * @param {char}    y  0~7
 * @param {uint8_t} A[]待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @param {uint8_t} select二维数组的第几元素
 * @return {*}
 */

void  OLED16x16BMP_fast(uint8_t x,char y,uint8_t select ,uint8_t Color_Turn ,uint8_t A[][16] )//数组赋值,40x40图片
{//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;	
	uint8_t m = 0;
	
	char x_bar=x;
	x=x_bar;//每行从该x位置起始
	for(j=0;j<16;j++)
	{  
		if(x >= 0 && x <= 127)//只在屏幕内显示
		{ 
			if(Color_Turn)
					Grame[y][x] = ~A[2*select][k];
			else 
				 Grame[y][x] =A[2*select][k];  
		}				
		x++;
		k++;//放在外面
	}
	x=x_bar;//每行从该x位置起始
	for(j=0;j<16;j++)
	{  
		if(x >= 0 && x <= 127)//只在屏幕内显示
		{ 
			if(Color_Turn)
					Grame[y+1][x] = ~A[2*select+1][m];
			else 
				 Grame[y+1][x] =A[2*select+1][m];  
		}				
		x++;
		m++;//放在外面
		
	}
}

/**
 * @function: void  OLEDCHinese_fast(uint8_t x,char y,uint8_t Color_Turn ,uint8_t select ,uint8_t size ,uint8_t Color_Turn )
 * @description: 在OLED特定区域显示16x16,24x24,32x32汉字
 * @param {uint8_t} x  0~127
 * @param {char}    y  0~7
 * @param {uint8_t} A[]待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @param {uint8_t} size 16 :16x16汉字;24 :24x24汉字;32 :32x32汉字
 * @param {uint8_t} select二维数组的第几元素
 * @return {*}
 */

void  OLEDCHinese_fast(uint8_t x,char y,uint8_t select ,uint8_t size ,uint8_t Color_Turn )
{//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	uint8_t m = 0;
	uint8_t a = 0;
	uint8_t b = 0;
	
	char x_bar=x;
/*16x16汉字*/	
	if(size==16)
	{
	x=x_bar;//每行从该x位置起始	
		for(j=0;j<16;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
					Grame[y][x] = ~Chinese16x16_fast[2*select][k];//汉字上半部分
        else 
				   Grame[y][x] =Chinese16x16_fast[2*select][k]; 			
			}				
			x++;
			k++;//放在外面
		}
		x=x_bar;//每行从该x位置起始		
		for(j=0;j<16;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		        Grame[y+1][x] = ~Chinese16x16_fast[2*select+1][m];//汉字下半部分
        else 
				   Grame[y+1][x] =Chinese16x16_fast[2*select+1][m]; 			
			}				
			x++;
			m++;
		}
	}
/*24x24汉字*/		
	else if(size==24)
	{
	 x=x_bar;//每行从该x位置起始	
		for(j=0;j<24;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
					Grame[y][x] = ~Chinese24x24_fast[3*select][k];//汉字1/3部分
        else 
				   Grame[y][x] =Chinese24x24_fast[3*select][k]; 			
			}				
			x++;
			k++;//放在外面
		}
		x=x_bar;//每行从该x位置起始		
		for(j=0;j<24;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		        Grame[y+1][x] = ~Chinese24x24_fast[3*select+1][m];//汉字2/3部分
        else 
				   Grame[y+1][x] =Chinese24x24_fast[3*select+1][m]; 			
			}				
			x++;
			m++;
		}
	 x=x_bar;//每行从该x位置起始	
		for(j=0;j<24;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
					Grame[y+2][x] = ~Chinese24x24_fast[3*select+2][a];//汉字3/3部分
        else 
				   Grame[y+2][x] =Chinese24x24_fast[3*select+2][a]; 			
			}				
			x++;
			a++;//放在外面
		}			
	}
/*32x32汉字*/			
	else if(size==32)
	{
	 x=x_bar;//每行从该x位置起始	
		for(j=0;j<32;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
					Grame[y][x] = ~Chinese32x32_fast[4*select][k];//汉字1/4部分
        else 
				   Grame[y][x] =Chinese32x32_fast[4*select][k]; 			
			}				
			x++;
			k++;//放在外面
		}
		x=x_bar;//每行从该x位置起始		
		for(j=0;j<32;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		        Grame[y+1][x] = ~Chinese32x32_fast[4*select+1][m];//汉字2/4部分
        else 
				   Grame[y+1][x] =Chinese32x32_fast[4*select+1][m]; 			
			}				
			x++;
			m++;
		}
	 x=x_bar;//每行从该x位置起始	
		for(j=0;j<32;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
					Grame[y+2][x] = ~Chinese32x32_fast[4*select+2][a];//汉字3/4部分
        else 
				   Grame[y+2][x] =Chinese32x32_fast[4*select+2][a]; 			
			}				
			x++;
			a++;//放在外面
		}
		x=x_bar;//每行从该x位置起始		
		for(j=0;j<32;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		        Grame[y+3][x] = ~Chinese32x32_fast[4*select+3][b];//汉字4/4部分
        else 
				   Grame[y+3][x] =Chinese32x32_fast[4*select+3][b]; 			
			}				
			x++;
			b++;
		}			
	}	
}
/**
 * @function: void  OLED32x32_fast(uint8_t x,char y,uint8_t Color_Turn ,uint8_t select ,uint8_t A[][16] )
 * @description: 在OLED特定区域显示24x24二维数组图片
 * @param {uint8_t} x  0~128
 * @param {char}    y  0~7
 * @param {uint8_t} A[]待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @param {uint8_t} select二维数组的第几元素
 * @return {*}
 */

void  OLED24x24_fast(uint8_t x,char y,uint8_t select ,uint8_t Color_Turn ,uint8_t A[][24] )//数组赋值,40x40图片
{//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;	
//	uint8_t x_bar = 0;	
	char x_bar=x;
	for(i=0;i<3;i++)
	{
		x=x_bar;//每行从该x位置起始
		k=0;
		for(j=0;j<24;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		          Grame[y][x] = ~A[3*select+i][k];
              else 
				   Grame[y][x] =A[3*select+i][k];  
			}				
			x++;
			k++;//放在外面
		}
		y++;
	}
}


/**
 * @function: void  OLED32x32_fast(uint8_t x,char y,uint8_t Color_Turn ,uint8_t select ,uint8_t A[][16] )
 * @description: 在OLED特定区域显示32x32二维数组图片
 * @param {uint8_t} x  0~128
 * @param {char}    y  0~7
 * @param {uint8_t} A[]待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @param {uint8_t} select二维数组的第几元素
 * @return {*}
 */

void  OLED32x32_fast(uint8_t x,char y,uint8_t select ,uint8_t Color_Turn ,uint8_t A[][32] )//数组赋值,40x40图片
{//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;	
//	uint8_t x_bar = 0;	
	char x_bar=x;
	for(i=0;i<4;i++)
	{
		x=x_bar;//每行从该x位置起始
		k=0;
		for(j=0;j<32;j++)
		{  
			if(x >= 0 && x <= 127)//只在屏幕内显示
			{ 
			  if(Color_Turn)
		        Grame[y][x] = ~A[4*select+i][k];
              else 
				Grame[y][x] =A[4*select+i][k];  
			}				
			x++;
			k++;//放在外面
		}
		y++;
	}
}



/********************************************************/
/**
 * @function: void OLED_DrawYaogan_left( uint8_t x_num,uint8_t y_num, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域0<x<40,1<y<6显示摇杆坐标框(左摇杆)
 * @param {uint8_t} x_num摇杆x轴方向x大小  0<x_num<40
 * @param {uint8_t} y_num摇杆y轴方向y大小  1<y_num<=5
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_DrawYaogan_left( uint8_t x_num,uint8_t y_num, uint8_t *  BMP,uint8_t Color_Turn)
{
  uint32_t j = 0,t=0;
  uint8_t x = 0, y = 0,n=0,current_position=0,biankuang=0;

	
  for(y=1;y<6;y++)
	{
		OLED_Set_Pos(0,y);			
	  	for(x=0;x<40;x++)
			{
				j++;
				if((((j-1)%40)>=x_num)&&(((j-1)%40)<(x_num+8))&&y_num==y)//j%40得出每行当前x的大小
				{           //y_num=y表示当前摇杆在Y方向上的位置与当前oled刷新的y方向的y值相等时
					 if(x_num==0&&y_num!=1&&y_num!=5)//摇杆摆到最左端时
					  OLED_WR_DATA(~BMP_dot6[t++]);//左边点 
					 else if(y_num==1&&x_num!=0)
						 OLED_WR_DATA(~BMP_dot[t++]);//上边点
					 else if(x_num==0&&y_num==1)
						 OLED_WR_DATA(~BMP_dot1[t++]);//上左点
					 else if(x_num==0&&y_num==5)
						 OLED_WR_DATA(~BMP_dot4[t++]);//下左点
					 else if(x_num!=0&&y_num==5)
						 OLED_WR_DATA(~BMP_dot3[t++]);//下边点	
					 else if(x_num==15&&y_num==3)
						 OLED_WR_DATA(~BMP_dot9[t++]);//过虚线中间点				 
					 else
					  OLED_WR_DATA(~BMP_dot7[t++]);//中间点
				}
				else
				{
					if (Color_Turn)
						OLED_WR_DATA(BMP[j-1]);//
					else
						OLED_WR_DATA(~BMP[j-1]);//(反相)													
					
				}
			}
	}

}
/**
 * @function: void OLED_DrawYaogan_right( uint8_t x_num,uint8_t y_num, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域87<x<127,1<y<6显示摇杆坐标框(右摇杆)
 * @param {uint8_t} x_num摇杆x轴方向x大小  87<x_num<127
 * @param {uint8_t} y_num摇杆y轴方向y大小  1<y_num<=5
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_DrawYaogan_right( uint8_t x_num,uint8_t y_num, uint8_t *  BMP,uint8_t Color_Turn)
{
  uint32_t j = 0,t=0;
  uint8_t x = 0, y = 0,n=0,current_position=0,biankuang=0;

	
  for(y=1;y<6;y++)
	{
		OLED_Set_Pos(87,y);	//起点位置		
	  	for(x=0;x<40;x++)
			{
				j++;
				if((((j-1)%40)>=x_num)&&(((j-1)%40)<(x_num+8))&&y_num==y)//j%40得出每行当前x的大小
				{           //y_num=y表示当前摇杆在Y方向上的位置与当前oled刷新的y方向的y值相等时
					 if(x_num==0&&y_num!=1&&y_num!=5)//摇杆摆到最左端时
					  OLED_WR_DATA(~BMP_dot6[t++]);//左边点 
					 else if(y_num==1&&x_num!=0)
						 OLED_WR_DATA(~BMP_dot[t++]);//上边点
					 else if(x_num==0&&y_num==1)
						 OLED_WR_DATA(~BMP_dot1[t++]);//上左点
					 else if(x_num==0&&y_num==5)
						 OLED_WR_DATA(~BMP_dot4[t++]);//下左点
					 else if(x_num!=0&&y_num==5)
						 OLED_WR_DATA(~BMP_dot3[t++]);//下边点	
					 else if(x_num==15&&y_num==3)
						 OLED_WR_DATA(~BMP_dot9[t++]);//过虚线中间点				 
					 else
					  OLED_WR_DATA(~BMP_dot7[t++]);//中间点
				}
				else
				{
					if (Color_Turn)
						OLED_WR_DATA(BMP[j-1]);//
					else
						OLED_WR_DATA(~BMP[j-1]);//(反相)													
					
				}
			}
	}

}



/*8x8图形*/
void OLED_DrawYaogan( uint8_t x_num,uint8_t y_num, uint8_t *  BMP,uint8_t Color_Turn)//显示一个点
{
	
  uint32_t j = 0;
  uint8_t x = 0, y = 7;
	
  for(y=0;y<1;y++)
	{
		OLED_Set_Pos(x_num,y_num);
//	  OLED_Set_Pos(x_num,y_num);//起点坐标
    for(x=0;x<8;x++)
		{
					if (Color_Turn)
						OLED_WR_DATA(BMP[x]);//显示能量条不变的部分
					else
					  OLED_WR_DATA(~BMP[x]);//显示能量条不变的部分(反相)			
		}
	}

}
void OLED_Clear_1(uint8_t x_num,uint8_t y_num)
{
	uint8_t i,n;
	for(i=y_num;i<y_num+1;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=x_num;n<x_num+8;n++)
			OLED_WR_DATA(0);
	}
}



void OLED_DrawBianchang( uint8_t x0, uint8_t y0,uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
{
	
  uint32_t j = 0;
  uint8_t x = 0, y = 7;
  for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
//	  OLED_Set_Pos(x_num,y_num);//起点坐标
    for(x=0;x<8;x++)
		{
					if (Color_Turn)
						OLED_WR_DATA(BMP[x]);//
					else
					  OLED_WR_DATA(~BMP[x]);//		
		}
	}

}


/**
 * @function: void OLED_HorizontalShift(uint8_t direction)
 * @description: 屏幕内容水平全屏滚动播放
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(uint8_t direction)

{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0x00);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频
    //  0x00-5帧， 0x01-64帧， 0x02-128帧， 0x03-256帧， 0x04-3帧， 0x05-4帧， 0x06-25帧， 0x07-2帧，
	OLED_WR_CMD(0x07);//设置结束页地址
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
 * @description: 屏幕部分内容水平滚动播放
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {uint8_t} start 开始页地址  0x00~0x07
 * @param {uint8_t} end  结束页地址  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(start);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频,0x07即滚动速度2帧
	OLED_WR_CMD(end);//设置结束页地址
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据

}

/**
 * @function: void OLED_VerticalAndHorizontalShift(uint8_t direction)
 * @description: 屏幕内容垂直水平全屏滚动播放
 * @param {uint8_t} direction				右上滚动	 0x29
 *                                                            左上滚动   0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x01);//虚拟字节设置
	OLED_WR_CMD(0x00);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频，即滚动速度
	OLED_WR_CMD(0x07);//设置结束页地址
	OLED_WR_CMD(0x01);//垂直滚动偏移量
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_DisplayMode(uint8_t mode)
 * @description: 屏幕内容取反显示
 * @param {uint8_t} direction			ON	0xA7  ，
 *                                                          OFF	0xA6	默认此模式，设置像素点亮
 * @return {*}
 */
void OLED_DisplayMode(uint8_t mode)
{
	OLED_WR_CMD(mode);
}

/**
 * @function: void OLED_IntensityControl(uint8_t intensity)
 * @description: 屏幕亮度调节
 * @param  {uint8_t} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(uint8_t intensity)
{
	OLED_WR_CMD(0x81);
	OLED_WR_CMD(intensity);
}

/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: 在OLED特定位置开始显示8*8图形
 * @param {uint8_t} x待显示的图形起始横坐标x: 0~112，两列图形之间需要间隔8
 * @param {uint8_t} y待显示的图形起始纵坐标 y: 0~6 , 两行图形之间需要间隔1
 * @param {uint8_t} no待显示的图形编号
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowGraph_8x8(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<8;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Txk_1[1*no][t]); //显示图形的上半部分
				else
					OLED_WR_DATA(Txk_1[1*no][t]); //显示图形的上半部分
        }

//		OLED_Set_Pos(x,y+1);
//    for(t=0;t<8;t++)
//		{
//				if (Color_Turn)
//					OLED_WR_DATA(~Txk_1[2*no+1][t]); //显示汉字的上半部分
//				else
//					OLED_WR_DATA(Txk_1[2*no+1][t]);  //显示汉字的上半部分
//         }
}
/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: 在OLED特定位置开始显示4*4图形
 * @param {uint8_t} x待显示的图形起始横坐标x: 0~112，两列图形之间需要间隔8
 * @param {uint8_t} y待显示的图形起始纵坐标 y: 0~6 , 两行图形之间需要间隔1
 * @param {uint8_t} no待显示的图形编号
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowGraph_3x3(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<3;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Txk_3[1*no][t]); //显示图形的上半部分
				else
					OLED_WR_DATA(Txk_3[1*no][t]); //显示图形的上半部分
        }

//		OLED_Set_Pos(x,y+1);
//    for(t=0;t<8;t++)
//		{
//				if (Color_Turn)
//					OLED_WR_DATA(~Txk_1[2*no+1][t]); //显示汉字的上半部分
//				else
//					OLED_WR_DATA(Txk_1[2*no+1][t]);  //显示汉字的上半部分
//         }
}

/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: 在OLED特定位置开始显示16*8图形
 * @param {uint8_t} x待显示的图形起始横坐标x: 0~112，两列图形之间需要间隔8
 * @param {uint8_t} y待显示的图形起始纵坐标 y: 0~6 , 两行图形之间需要间隔1
 * @param {uint8_t} no待显示的图形编号
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowGraph_16x8(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Txk_2[1*no][t]); //显示图形的上半部分
				else
					OLED_WR_DATA(Txk_2[1*no][t]); //显示图形的上半部分
        }
}

