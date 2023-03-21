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
    char bombe_code[5] = "    ";
    char input_code[5] = "    ";
    int planting_sec;
    bool input_try = false;
    int state;
    void plant();
};

#endif