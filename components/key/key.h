/*******************************************
# Author        : Retuze
# Date          : 2023-10-31 02:51:33
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-19 18:44:06
# Description   :
*********************************************/
#ifndef _KEY_H_
#define _KEY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#ifdef __cplusplus
}
#endif

class KEY
{
public:
    KEY(uint8_t (*getKeyState)())
    {
        this->getKeyState = getKeyState;
        state = UP;
    }
    /// @brief 每20ms调用一次
    void run()
    {
        static uint8_t long_event_down_flag = 0;
        // 首次检测到按下
        if (getKeyState() == DOWN && long_tick == 0)
        {
            // 按键被按下
            state = DOWN;
            long_tick++;
        }
        // 检测到一直按下
        else if (getKeyState() == DOWN && long_tick != 0)
        {
            long_tick++;
            if (long_tick > 25)
            {
                if (long_event != nullptr && long_event_repeat == nullptr && long_event_down_flag == 0)
                {
                    // 如果长按事件返回值为true则触发短按事件
                    if (long_event())
                    {
                        if (short_event != nullptr)
                        {
                            short_event();
                        }
                    }
                    long_event_down_flag = 1;
                }
                else if (long_event_repeat != nullptr)
                {
                    if ((long_tick - 25) % 10 == 0)
                    {
                        long_event_repeat();
                    }
                }
            }
        }
        // 首次检测到从按下变为抬起
        else if (getKeyState() == UP && state == DOWN)
        {
            // 按下时间不超过500ms，正常触发短按事件
            if (long_tick <= 25)
            {
                if (short_event != nullptr)
                {
                    short_event();
                }
            }
            long_event_down_flag = 0;
            long_tick = 0;
            state = UP;
        }
    }
    void setOnClickEvent(void (*short_event)())
    {
        this->short_event = short_event;
    }
    void setOnLongClickEvent(bool (*long_event)())
    {
        this->long_event = long_event;
    }
    void setOnLongClickEventRepeat(void (*long_event_repeat)())
    {
        this->long_event_repeat = long_event_repeat;
    }

private:
    enum KEY_STATE
    {
        UP,
        DOWN,
    } state;
    uint32_t tick;
    uint32_t long_tick;
    /// @brief 返回按下或者抬起状态
    uint8_t (*getKeyState)();
    /// @brief 短按事件
    void (*short_event)();
    /// @brief 长按事件，返回值为true时，会自动触发短按事件，false则不触发短按事件
    bool (*long_event)();
    /// @brief 长按连发事件，如果定义了则c
    void (*long_event_repeat)();
};

#endif // _KEY_H_
