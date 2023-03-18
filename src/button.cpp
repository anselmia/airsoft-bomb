#include "button.h"

BUTTON::BUTTON(int pin)
{
    buttonPin = pin;
    buttonState = NOT_PRESSED;
}

void BUTTON::readButton()
{
    int selectedButton = digitalRead(buttonPin);
    Serial.print("Pin ");
    Serial.print(buttonPin);
    Serial.print(" input state : ");
    Serial.print(selectedButton);
    Serial.println(F(""));
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
            buttonState = PRESSED;
        }
        else
        {
            buttonState = NOT_PRESSED;
        }
        break;
    case PRESSED:
        if (selectedButton == 1)
        {
            buttonState = NOT_PRESSED;
        }
    }
}
