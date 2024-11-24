#ifndef OLED_OLED_H_
#define OLED_OLED_H_

#include "stm32l0xx_hal.h"
#include "oledfont.h"
extern I2C_HandleTypeDef  hi2c1;
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

void OLED_WR_CMD(u8 cmd);
void OLED_WR_DATA(u8 data);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_On(void);
void OLED_ShowNum(u8 x,u8 y,unsigned int num,u8 len,u8 size2,u8 Color_Turn);
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2, u8 Color_Turn);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 Color_Turn);
void OLED_ShowString(u8 x,u8 y,char*chr,u8 Char_Size,u8 Color_Turn);
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 Color_Turn);
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 *  BMP,u8 Color_Turn);
void OLED_HorizontalShift(u8 direction);
void OLED_Some_HorizontalShift(u8 direction,u8 start,u8 end);
void OLED_VerticalAndHorizontalShift(u8 direction);
void OLED_DisplayMode(u8 mode);
void OLED_IntensityControl(u8 intensity);

void OLED_ShowGraph_8x8(u8 x,u8 y,u8 no, u8 Color_Turn);
void OLED_ShowGraph_16x8(u8 x,u8 y,u8 no, u8 Color_Turn);
void OLED_ShowGraph_3x3(u8 x,u8 y,u8 no, u8 Color_Turn);
	
//void OLED_DrawNengLiangtiao(u8 num, u8 *  BMP,u8 Color_Turn);
void OLED_DrawNengLiangtiao( u8 y0, u8 y1,u8 num, u8 *  BMP,u8 Color_Turn);//?????????
void OLED_DrawNengLiangtiao_1(u8 y0, u8 y1,u8 num, u8 *  BMP,u8 Color_Turn);//?????????
void OLED_DrawYaogan( u8 x_num,u8 y_num, u8 *  BMP,u8 Color_Turn);
void OLED_DrawBianchang( u8 x0, u8 y0,u8 y1, u8 *  BMP,u8 Color_Turn);
void OLED_Clear_1(u8 x_num,u8 y_num);
void OLED_DrawYaogan_left( u8 x_num,u8 y_num, u8 *  BMP,u8 Color_Turn);
void OLED_DrawYaogan_right( u8 x_num,u8 y_num, u8 *  BMP,u8 Color_Turn);


void  OLED_40x40BMP_fast(long x,char y,u8 select,u8 Color_Turn ,u8 A[][40] );
void  OLED8x8BMP_fast(u8 x,char y,u8 select ,u8 Color_Turn ,u8 A[][8] );
void  OLED16x16BMP_fast(u8 x,char y,u8 select ,u8 Color_Turn ,u8 A[][16] );
void  OLEDCHinese_fast(u8 x,char y,u8 select ,u8 size ,u8 Color_Turn );
void  OLED24x24_fast(u8 x,char y,u8 select ,u8 Color_Turn ,u8 A[][24] );

void  OLED32x32_fast(u8 x,char y,u8 select ,u8 Color_Turn ,u8 A[][32] );//

void UI_Screen(void);
void move_bmp(void );
void OLED_Refresh(u8 mod);

void OLED_Refresh_1(void);
u8 Draw_pot(u8 x,u8 y);
u8 Clear_pot(u8 x,u8 y);
static u8 Pos_Target(u8 *x,u8 x_target);
u8 Draw_line(u8 x_start,u8 y_start,u8 x_end,u8 y_end);
u8 Clear_Draw_line(u8 x_start,u8 y_start,u8 x_end,u8 y_end);
void Draw_square(long x_start,long y_start,u8 length,u8 width);
void Clear_square(u8 x_start,u8 y_start,u8 length,u8 weighth);
void OLED_DrawCircle_lest(int x,int y,u8  r);
void OLED_DrawCircle_better(int x0,int y0,u8 r);
void OLED_DrawCircle_most(u8 x0,u8 y0,u8 r);
void OLED_DrawFullCircle(u8 x0,u8 y0,u8 r);
//void OLED_StatUpdate(void );
void progress_right(u8  x,u8 y,u8 length,u8 width,u8 value);
void progress_left(u8  x,u8 y,u8 length,u8 width,u8 value);
void progress_up(u8  x,u8 y,u8 length,u8 width,u8 value);
void progress_down(u8  x,u8 y,u8 length,u8 width,u8 value);

void line(long x,long y,char length,u8 mode);
void Clear_line(u8 x,u8 y,u8 length,u8 mode);

void drawline(float x1,float y1,float x2,float y2);
void Clear_drawline(float x1,float y1,float x2,float y2);

void OLED_DrawLine(int x0, int y0, int x1, int y1);
void Clear_OLED_DrawLine(int x0, int y0, int x1, int y1);
void OLED_flash_8_Fast(u8 x,u8 y,u8 n,u8 A[][8]);
u8 OLED_flash_Show_Char_8(u8 x,u8 y,u8 C);
void OLED_flash_String_8(u8 x,u8 y,char *C);
void OLED_flash_6_Fast(u8 x,u8 y,u8 n,u8 A[][6]);
u8 OLED_flash_Show_Char_6(u8 x,u8 y,u8 C);
void OLED_flash_String_6(u8 x,u8 y,char *C);

void OLED_DrawPoint(int x,int y,u8 t);
void OLED_ShowChar_Fast(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowdecimalNumber_Fast(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size,u8 mode);
void OLED_ShowNumber_Fast(u8 x,u8 y,int num,u8 len,u8 size,u8 mode);
void OLED_ShowString_Fast(u16 x,u8 y,char *p,u8 size,u8 mode,u8 line_variable);
void OLED_ShowCH_Fast(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_Show_CH_Fast(u8 x,u8 y,u8 chr,u8 size,u8 mode);
u8 OLED_findoneCN_Fast(u8 ch1,u8 ch2,u8 size);
void OLED_Show_chString_Fast(u8 x, u8 y,u8 ch[],u8 size);
void ellipse(int x,int y,uint8_t major_semi_axis,uint8_t minor_semi_axis,float t);
void OLED_invert(int x,int y);
void OLED_invert_line(u8 x,u8 y,u8 length,u8 mode);
void OLED_invert_full(u8 x,u8 y,u8 width,u8 length);
void OLED_sel(float x,float y,float len);
void OLED_sel_time(float x,float y,float len);
void OLED_sel_wave(float x,float y,float len);
void OLED_sel_avoid(float x,float y,float len);
void OLED_sel_follow(float x,float y,float len);

void OLED_BMP_slide(float x,float y,uint8_t select);
uint16_t OLED_slide(float m,uint8_t move_num);
uint16_t OLED_slide_name(float m,uint8_t move_num);
uint16_t OLED_slide_light(float m,uint8_t move_num);
uint16_t OLED_slidenum_light(float m,uint8_t move_num);

void pro_up(u8 x,u8 y,u8 length,u8 value);

void left_or_right_Throttle_fast(uint8_t x,uint8_t y,uint16_t variable);//◊Û”Õ√≈
void Slide_point(u8 x,u8 y,u8 length,u8 width,u16 var_max,u16 variable);
void progress_bar(u8 x,u8 y,u8 length,u8 width,u16 variable);
void rocher_position(long x,long y,u8 side,u8 variable_x,u8 variable_y);
void propulsion_strip(long x,u8 y,u8 select);
void draw_wave(u8 x,u8 x1,u8 y,u8 width_x,u8 hight_y,uint16_t variable);


#endif /* OLED_OLED_H_ */

