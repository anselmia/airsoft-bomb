#include "button.h"
#include "Arduino.h" // to remove

BUTTON::BUTTON(int pin)
{
    buttonPin = pin;
    buttonState = NOT_PRESSED;
    pinMode(pin, INPUT_PULLUP);
}

void BUTTON::readButton()
{
    int selectedButton = digitalRead(buttonPin);

    switch (buttonState)
    {
    case NOT_PRESSED:
        if (selectedButton == 0)
        {
            buttonState = PUSHED;
        }
        break;
    case PUSHED:
        if (selectedButton == 0)
        {
            buttonState = KEY_PRESSED;
        }
        else
        {
            buttonState = NOT_PRESSED;
        }
        break;
    case KEY_PRESSED:
        if (selectedButton == 1)
        {
            buttonState = NOT_PRESSED;
        }
    }
}