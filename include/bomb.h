#ifndef BOMB_h
#define BOMB_h
#include <Arduino.h>

static const byte UNPLANTED = 0;
static const byte ONGOING = 1;
static const byte PLANTED = 2;
static const byte EXPLODED = 3;
static const byte DEFUSED = 4;

class BOMB
{
private:
    unsigned long currmillis;
    unsigned long prevMillis;

public:
    BOMB();
    bool defused;
    bool boom;
    int bombe_code[5] = {0, 0, 0, 0};
    int input_code[5] = {0, 0, 0, 0};
    unsigned long plantmillis;
    int planting_sec;
    bool input_try = false;
    int state;
    void plant();
};

#endif