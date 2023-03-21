#include "bomb.h"

// Before to start verify all wire coonected
BOMB::BOMB()
{
    state = UNPLANTED;
    planting_sec = 0;
}

void BOMB::plant()
{
    if (planting_sec == 0)
        plantmillis = millis();
    state = ONGOING;
    currmillis = (millis() / 1000.0); // currmilis== current milliseconds

    // next every time when secs is equal to 0 minutes are increased by 1 initial value of minutes is -1
    if (currmillis > prevMillis) // use minlock because otherwise when secs==0 mins increase more than +1
    {
        prevMillis = currmillis;
        planting_sec++;
        if (planting_sec == 10)
        {
            state = PLANTED;
            planting_sec = 0;
        }
    }
}
