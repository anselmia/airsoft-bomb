#include "key.h"

KEY::KEY(int k)
{
    key = k;
    buttonState = NOT_PRESSED;
}

void KEY::readButton()
{
    ttp229.readKeys();

    if (key == ttp229.getKey())
    {
        switch (buttonState)
        {
        case NOT_PRESSED:
            buttonState = PUSHED;
        case PUSHED:
            buttonState = PRESSED;
        }
    }
    else
        buttonState = NOT_PRESSED;
}
