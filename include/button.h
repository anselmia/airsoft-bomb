#ifndef BUTTON_h
#define BUTTON_h
#include <Arduino.h>

static const byte NOT_PRESSED = 0;
static const byte PUSHED = 1;
static const byte PRESSED = 2;

class BUTTON
{
private:
    int buttonPin;

public:
    BUTTON(int pin);
    byte buttonState;
    void readButton();
};

#endif