#ifndef KEY_h
#define KEY_h
#include "button.h"
#include <Arduino.h>

static const byte key_0 = 14;
static const byte key_1 = 1;
static const byte key_2 = 2;
static const byte key_3 = 3;
static const byte key_4 = 5;
static const byte key_5 = 6;
static const byte key_6 = 7;
static const byte key_7 = 9;
static const byte key_8 = 10;
static const byte key_9 = 11;
static const byte key_A = 4;
static const byte key_B = 8;
static const byte key_C = 12;
static const byte key_D = 16;
static const byte key_star = 13;
static const byte key_hash = 15;

// static const byte key_0 = 48;
// static const byte key_1 = 49;
// static const byte key_2 = 50;
// static const byte key_3 = 51;
// static const byte key_4 = 52;
// static const byte key_5 = 53;
// static const byte key_6 = 54;
// static const byte key_7 = 55;
// static const byte key_8 = 56;
// static const byte key_9 = 57;
// static const byte key_A = 97;
// static const byte key_B = 98;
// static const byte key_C = 99;
// static const byte key_D = 100;
// static const byte key_star = 42;
// static const byte key_hash = 47;

class KEY
{
private:

public:
    KEY(int k);
    int key;
    byte buttonState;
    // void readButton(int selected_key);
    void readButton(int k);
};

#endif