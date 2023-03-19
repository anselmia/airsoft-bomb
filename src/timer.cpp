#include "timer.h"
#include "Arduino.h"

TIMER::TIMER(int sec, int min)
{
    timer_step = 1000;
    secs = sec;
    mins = min;
}

void TIMER::updateTime()
{
    currmillis = millis();
    if (currmillis - prevMillis > timer_step)
        prevMillis = currmillis;

    if (secs <= 0 && mins > 0)
    {
        secs = 59;
        mins = mins - 1;
    }

    if (mins < 0)
        mins = 0;
    if (secs < 0)
        secs = 0;
}

void TIMER::penalty()
{
    timer_step = timer_step / 2;
    int min_to_sec = (mins * 60) + secs;
    int penalty = min_to_sec / 2;
    min_to_sec = min_to_sec - penalty;
    secs = min_to_sec % 60;
    mins = min_to_sec / 60;
}

void TIMER::boom()
{
    secs = 0;
    mins = 0;
}