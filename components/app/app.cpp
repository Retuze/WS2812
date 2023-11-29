/*******************************************
# Author        : Retuze 
# Date          : 2023-11-29 19:55:32
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-29 20:13:48
# Description   : 
*********************************************/
#include "app.h"
#include "key.h"
#include "ws2812.h"

KEY key_switch([]()->uint8_t{
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
});

KEY key_off([]()->uint8_t{
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8);
});

void app_lunch()
{
    RGB_BLUE(8);
    key_switch.setOnClickEvent([](){
        static uint8_t color = 0;
        switch (color)
        {
        case 0:
            RGB_WHITE(8);
            break;
        case 1:
            RGB_RED(8);
            break;
        case 2:
            RGB_GREEN(8);
            break;
        case 3:
            RGB_YELLOW(8);
            break;
        default:
            break;
        }
        color++;
        if(color>3)
        {
            color=0;
        }
    });
    key_off.setOnClickEvent([](){
        RGB_BLACK(8);
    });
    while (1)
    {
        static uint32_t tick =0;
        if(HAL_GetTick()-tick>20)
        {
            tick=HAL_GetTick();
            key_switch.run();
            key_off.run();
        }
    }
    
}