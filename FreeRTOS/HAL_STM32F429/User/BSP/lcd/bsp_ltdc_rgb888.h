#ifndef __BSP_LCD_H
#define	__BSP_LCD_H
#include "board.h"
// #include "./fonts/fonts.h"

/* LCD �����ظ�ʽ*/
#define ARGB8888 	LTDC_PIXEL_FORMAT_ARGB8888  /*!< ARGB8888 LTDC���ظ�ʽ */
#define RGB888 		LTDC_PIXEL_FORMAT_RGB888    /*!< RGB888 LTDC���ظ�ʽ   */
#define RGB565 		LTDC_PIXEL_FORMAT_RGB565    /*!< RGB565 LTDC���ظ�ʽ   */
#define ARGB1555 	LTDC_PIXEL_FORMAT_ARGB1555  /*!< ARGB1555 LTDC���ظ�ʽ */
#define ARGB4444 	LTDC_PIXEL_FORMAT_ARGB4444  /*!< ARGB4444 LTDC���ظ�ʽ */

/** 
  * @brief  LCDҺ������
  */   
typedef struct
{
  /*����Һ�������ֲ�Ĳ�������*/
  uint8_t hbp;  //HSYNC�����Ч����
  uint8_t vbp;  //VSYNC�����Ч����

  uint8_t hsw;  	//HSYNC���
  uint8_t vsw;   //VSYNC���

  uint8_t hfp;  	//HSYNCǰ����Ч����
  uint8_t vfp;  	//VSYNCǰ����Ч����
  
  uint8_t comment_clock_2byte; //rgb565/argb4444��˫�ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
  uint8_t comment_clock_4byte; //Argb8888�����ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��

  uint16_t lcd_pixel_width; //Һ���ֱ��ʣ���
  uint16_t lcd_pixel_height;//Һ���ֱ��ʣ���

}LCD_PARAM_TypeDef;

/* LCD �������ش�С (��Ⱥ͸߶�) */
#define  LCD_PIXEL_WIDTH          lcd_param[cur_lcd].lcd_pixel_width
#define  LCD_PIXEL_HEIGHT         lcd_param[cur_lcd].lcd_pixel_height

/*����Һ�������ֲ�Ĳ�������*/
#define HBP  lcd_param[cur_lcd].hbp		//HSYNC�����Ч����
#define VBP  lcd_param[cur_lcd].vbp		//VSYNC�����Ч����

#define HSW  lcd_param[cur_lcd].hsw		//HSYNC���
#define VSW  lcd_param[cur_lcd].vsw		//VSYNC���

#define HFP  lcd_param[cur_lcd].hfp		//HSYNCǰ����Ч����
#define VFP  lcd_param[cur_lcd].vfp		//VSYNCǰ����Ч����

/** 
  * @brief  LCDҺ������
  */   
typedef enum
{ 
  INCH_5  = 0x00, /* Ұ��5���� */
  INCH_7,     /* Ұ��7���� */
  INCH_4_3,  /* Ұ��4.3���� */
  
  LCD_TYPE_NUM /* LCD��������*/
}LCD_TypeDef;

/* ��ǰʹ�õ�LCD��Ĭ��Ϊ5���� */
extern LCD_TypeDef cur_lcd;
/* ��ͬҺ�����Ĳ��� */
extern const LCD_PARAM_TypeDef lcd_param[];


typedef struct 
{ 
  uint32_t TextColor; 
  uint32_t BackColor;  
  sFONT    *pFont;
}LCD_DrawPropTypeDef;   
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 

/** 
  * @brief  �������ģʽ  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* ���ж��� */
  RIGHT_MODE              = 0x02,    /* �Ҷ���   */
  LEFT_MODE               = 0x03     /* �����   */
}Text_AlignModeTypdef;

#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */
/** 
  * @brief  LCD status structure definition  
  */     
#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)
#define LCD_TIMEOUT            ((uint8_t)0x02)

/** 
  * @brief  LCD FB_StartAddress  
  */
#define LCD_FB_START_ADDRESS       ((uint32_t)0xD0000000)
/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT        Font24 



void  LCD_Init(void);
uint8_t  LCD_DeInit(void);
uint32_t LCD_GetXSize(void);
uint32_t LCD_GetYSize(void);
void     LCD_SetXSize(uint32_t imageWidthPixels);
void     LCD_SetYSize(uint32_t imageHeightPixels);

/* Functions using the LTDC controller */
void     LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
void     LCD_LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address);
void     LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
void     LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
void     LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
void     LCD_ResetColorKeying(uint32_t LayerIndex);
void     LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void     LCD_SelectLayer(uint32_t LayerIndex);
void     LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State);

void     LCD_SetTextColor(uint32_t Color);
uint32_t LCD_GetTextColor(void);
void     LCD_SetBackColor(uint32_t Color);
uint32_t LCD_GetBackColor(void);

void LCD_SetColors(uint32_t TextColor, uint32_t BackColor);
    
void     LCD_SetFont(sFONT *fonts);
sFONT    *LCD_GetFont(void);

uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t pixel);
void     LCD_Clear(uint32_t Color);
void     LCD_ClearLine(uint32_t Line);
void     LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void     LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void     LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void     LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);

void     LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void     LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     LCD_DisplayOff(void);
void     LCD_DisplayOn(void);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void     LCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params);

void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat);

void Lcd_RGB888_Cfg(void);
void LCD_Test(void);
#endif /* __BSP_LCD_H */
