#ifndef BOMB_h
#define BOMB_h
#include <Arduino.h>

static const byte UNPLANTED = 0;
static const byte ONGOING = 1;
static const byte PLANTED = 2;
static const byte EXPLODED = 3;
static const byte DIFFUSED = 4;

class BOMB
{
private:
    unsigned long currmillis;
    unsigned long prevMillis;    

public:
    BOMB();
    int planting_sec;
    int state;
    void plant();
};

#endif