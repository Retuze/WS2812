/*******************************************
# Author        : Retuze
# Date          : 2023-11-29 19:41:08
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-29 20:03:47
# Description   :
*********************************************/
#ifndef _WS2812_H_
#define _WS2812_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct // 颜色结构体
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGBColor_TypeDef;

#define RGB_NUM 8*8 // RGB灯的数量，即为缓冲区长度

// 复位函数
void RGB_RST(void);
// 颜色设置函数
void RGB_Set_Color(uint8_t LedId, RGBColor_TypeDef Color);
// RGB 刷新函数
void RGB_Reflash(uint8_t reflash_num);

// 各种颜色测试
void RGB_RED(uint16_t RGB_LEN);     // 红
void RGB_GREEN(uint16_t RGB_LEN);   // 绿
void RGB_BLUE(uint16_t RGB_LEN);    // 蓝
void RGB_YELLOW(uint16_t RGB_LEN);  // 黄
void RGB_MAGENTA(uint16_t RGB_LEN); // 紫
void RGB_BLACK(uint16_t RGB_LEN);   // 黑
void RGB_WHITE(uint16_t RGB_LEN);   // 白

#ifdef __cplusplus
}
#endif

#endif // _WS2812_H_