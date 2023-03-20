#ifndef KEY_h
#define KEY_h
#include "button.h"
#include <Arduino.h>
#include <TTP229.h>

// static const byte but_0 = 0;
// static const byte but_1 = 1;
// static const byte but_2 = 2;
// static const byte but_3 = 3;
// static const byte but_4 = 4;
// static const byte but_5 = 5;
// static const byte but_6 = 6;
// static const byte but_7 = 7;
// static const byte but_8 = 8;
// static const byte but_9 = 9;
// static const byte but_A = 10;
// static const byte but_B = 11;
// static const byte but_C = 12;
// static const byte but_D = 13;
// static const byte but_star = 14;
// static const byte but_hash = 15;

static const byte but_0 = 48;
static const byte but_1 = 49;
static const byte but_2 = 50;
static const byte but_3 = 51;
static const byte but_4 = 52;
static const byte but_5 = 53;
static const byte but_6 = 54;
static const byte but_7 = 55;
static const byte but_8 = 56;
static const byte but_9 = 57;
static const byte but_A = 97;
static const byte but_B = 98;
static const byte but_C = 99;
static const byte but_D = 100;
static const byte but_star = 42;
static const byte but_hash = 47;

class KEY
{
private:
    TTP229 ttp229;
    // Capacitive KeyPad 8229BSF

public:
    KEY(int k);
    int key;
    byte buttonState;
    void readButton();
};

#endif