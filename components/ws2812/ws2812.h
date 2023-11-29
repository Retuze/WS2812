/*******************************************
# Author        : Retuze
# Date          : 2023-11-29 19:41:08
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-29 21:40:12
# Description   :
*********************************************/
#ifndef _WS2812_H_
#define _WS2812_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
}
#endif

class WS2812
{
private:
    // 缓冲区
    uint8_t *pbuf;
    // 灯的数量
    uint8_t num;
    // spi发送函数
    void (*spi_send)(uint8_t *pbuf, uint8_t len);
    typedef struct // 颜色结构体
    {
        uint8_t R;
        uint8_t G;
        uint8_t B;
    } RGBColor_TypeDef;
    // 模拟bit码:0xC0 为 0,0xF8 为 1
    const uint8_t code[2] = {0xC0, 0xF8};

public:
    // 常用的颜色，亮度调的比较低
    const RGBColor_TypeDef RED = {30, 0, 0};
    const RGBColor_TypeDef GREEN = {0, 50, 0};
    const RGBColor_TypeDef BLUE = {0, 0, 100};
    const RGBColor_TypeDef YELLOW = {30, 30, 0};
    const RGBColor_TypeDef MAGENTA = {30, 0, 30};
    const RGBColor_TypeDef BLACK = {0, 0, 0};
    const RGBColor_TypeDef WHITE = {80, 80, 80};

    /// @brief 初始化函数
    /// @param num 需要控制的灯珠数量
    /// @param spi_send spi发送函数
    WS2812(uint8_t num, void (*spi_send)(uint8_t *pbuf, uint8_t len))
    {
        this->num = num;
        this->spi_send = spi_send;
        pbuf = (uint8_t *)malloc(num * 8 * 3);
    }

    ~WS2812()
    {
        free(pbuf);
        pbuf = NULL;
    }

    const RGBColor_TypeDef *RGB(uint8_t R, uint8_t G, uint8_t B)
    {
        static RGBColor_TypeDef rgb_t = {0, 0, 0};
        rgb_t.R = R;
        rgb_t.G = G;
        rgb_t.B = B;
        return &rgb_t;
    }

    /// @brief 设置一颗灯珠的颜色
    /// @param LedId 该灯珠的id，第一个灯珠id为0，下一个id加一，以此类推
    /// @param rgb_t 设置的rgb值，如RGB(255,255,255)
    void setColor(uint8_t LedId, const RGBColor_TypeDef *rgb_t)
    {
        if (LedId < num)
        {
            uint8_t dat_r = rgb_t->R;
            uint8_t dat_g = rgb_t->G;
            uint8_t dat_b = rgb_t->B;
            for (int j = 0; j < 8; j++)
            {
                *(pbuf+LedId*24+7-j) = code[dat_g & 0x01];
                *(pbuf+LedId*24+15 - j) = code[dat_r & 0x01];
                *(pbuf+LedId*24+23 - j) = code[dat_b & 0x01];
                dat_g >>= 1;
                dat_r >>= 1;
                dat_b >>= 1;
            }
        }
    }

    /// @brief 设置一定数目的灯珠颜色
    /// @param LedId 起始灯珠的id，第一个灯珠id为0，下一个id加一，以此类推
    /// @param rgb_t 设置的rgb值，如RGB(255,255,255)
    /// @param num 灯珠数量
    void setColor(uint8_t LedId, const RGBColor_TypeDef *rgb_t, uint8_t num)
    {
        while (LedId < this->num && LedId < num)
        {
            setColor(LedId, rgb_t);
            LedId++;
        }
    }

    /// @brief 把全部灯珠设置为某个颜色
    /// @param rgb_t 设置的rgb值，如RGB(255,255,255)
    void setColor(const RGBColor_TypeDef *rgb_t)
    {
        uint8_t LedId = 0;
        while (LedId < this->num)
        {
            setColor(LedId, rgb_t);
            LedId++;
        }
    }

    /// @brief 清空缓冲区
    void cleanBuffer()
    {
        memset(pbuf, 0, num * 3 * 8);
    }

    /// @brief 把缓冲区发送到WS2812
    void sendBuffer()
    {
        spi_send(pbuf, num * 3 * 8);
    }
};

#endif // _WS2812_H_