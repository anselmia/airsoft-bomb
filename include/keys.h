#ifndef KEYS_h
#define KEYS_h
#include "button.h"
#include <Arduino.h>

#include <I2CKeyPad.h>

#define I2CADDR 0x20 // Set the Address of the PCF8574

// Set the Key at Use (4x4)
inline char layout_keypad[19] = "123A456B789C*0#DNF"; // N = NoKey, F = Fail

// Keyboard 4x4
static const byte key_0 = '0';
static const byte key_1 = '1';
static const byte key_2 = '2';
static const byte key_3 = '3';
static const byte key_4 = '4';
static const byte key_5 = '5';
static const byte key_6 = '6';
static const byte key_7 = '7';
static const byte key_8 = '8';
static const byte key_9 = '9';
static const byte key_A = 'A';
static const byte key_B = 'B';
static const byte key_C = 'C';
static const byte key_D = 'D';
static const byte key_star = '*';
static const byte key_hash = '#';

// Serial keyboard
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

class KEYS
{
private:
public:
    KEYS(char k);
    char key;
    byte keyState;
    void readKey(char k);
};

#endif