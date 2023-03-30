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
    if ((int)(currmillis - prevMillis) > timer_step)
    {
        prevMillis = currmillis;
        secs--;
    }

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

void TIMER::increase_min()
{
    mins += 1;
    if (mins > 60)
        mins = 0;
}

void TIMER::decrease_min()
{
    mins -= 1;
    if (mins < 0)
        mins = 60;
}

void TIMER::increase_sec()
{
    secs += 15;
    if (secs > 60)
        secs = 0;
}

void TIMER::decrease_sec()
{
    secs -= 15;
    if (secs < 0)
        secs = 60;
}

void TIMER::step_penalty(int divider)
{
    timer_step = timer_step / divider;
}

void TIMER::time_penalty(int divider)
{
    int min_to_sec = (mins * 60) + secs;
    int penalty = min_to_sec / divider;
    min_to_sec = min_to_sec - penalty;
    secs = min_to_sec % 60;
    mins = min_to_sec / 60;
}

void TIMER::boom()
{
    secs = 0;
    mins = 0;
}