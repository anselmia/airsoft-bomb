#include "bomb.h"

// Before to start verify all wire coonected
BOMB::BOMB()
{
    state = UNPLANTED;
    planting_sec = 0;
    defused = false;
    boom = false;
}

void BOMB::plant()
{
    if (planting_sec == 0)
    {
        plantmillis = millis();
        prevMillis = 0;
    }
    state = ONGOING;
    currmillis = (millis() / 1000.0);

    if (currmillis > prevMillis)
    {
        prevMillis = currmillis;
        planting_sec++;
        if (planting_sec > 1)
        {
            state = PLANTED;
            planting_sec = 0;
        }
    }
}
