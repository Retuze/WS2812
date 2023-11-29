/*******************************************
# Author        : Retuze
# Date          : 2023-11-29 19:55:32
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-29 21:45:21
# Description   :
*********************************************/
#include "app.h"
#include "key.h"
#include "dma.h"
#include "spi.h"
#include "ws2812.h"

KEY key_switch([]() -> uint8_t
{ 
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10); 
});

KEY key_off([]() -> uint8_t
{ 
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8); 
});

WS2812 ws2812(8, [](uint8_t *pbuf, uint8_t len)
{
    /* 判断上次DMA有没有传输完成 */
    while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY)
    {

    }
    /* 发送 RGB 数据到 2812 */
    HAL_SPI_Transmit_DMA(&hspi1, pbuf, len);
});

void app_lunch()
{
    ws2812.cleanBuffer();
    ws2812.setColor(&ws2812.BLUE);
    ws2812.sendBuffer();
    key_switch.setOnClickEvent([]()
    {
        static uint8_t color = 0;
        switch (color)
        {
        case 0:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.RED);
            ws2812.sendBuffer();
            break;
        case 1:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.GREEN);
            ws2812.sendBuffer();
            break;
        case 2:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.BLUE);
            ws2812.sendBuffer();
            break;
        case 3:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.YELLOW);
            ws2812.sendBuffer();
            break;
        case 4:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.MAGENTA);
            ws2812.sendBuffer();
            break;
        case 5:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.WHITE);
            ws2812.sendBuffer();
            break;
        case 6:
            ws2812.cleanBuffer();
            ws2812.setColor(&ws2812.BLACK);
            ws2812.sendBuffer();
            break;
        default:
            break;
        }
        color++;
        if(color>6)
        {
            color=0;
        } });
    key_off.setOnClickEvent([]()
    { 
        ws2812.cleanBuffer();
        ws2812.setColor(&ws2812.BLACK);
        ws2812.sendBuffer(); 
    });
    while (1)
    {
        static uint32_t tick = 0;
        if (HAL_GetTick() - tick > 20)
        {
            tick = HAL_GetTick();
            key_switch.run();
            key_off.run();
        }
    }
}