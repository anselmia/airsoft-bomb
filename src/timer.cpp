#include "timer.h"
#include "Arduino.h"

TIMER::TIMER(int sec, int min)
{
    decrement = 1;
    secs = sec;
    mins = min;
}

void TIMER::updateTime()
{
    // obtain seconds from arduino time
    currmillis = (millis() / 1000.0); // currmilis== current milliseconds

    if (currmillis > prevMillis)
    {
        prevMillis = currmillis;
        secs -= decrement;
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

void TIMER::penalty()
{
    decrement++;
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