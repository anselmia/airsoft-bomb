#include "keys.h"

KEYS::KEYS(char k)
{
    key = k;
    keyState = NOT_PRESSED;
}

// 4x4
void KEYS::readKey(char k)
{
    if (key == k)
    {
        switch (keyState)
        {
        case NOT_PRESSED:
            keyState = PUSHED;
        case PUSHED:
            keyState = PRESSED;
        }
    }
    else
        keyState = NOT_PRESSED;
}

// to remove
// void KEY::readButton(int selectedkey)
//{
//    if (key == selectedkey)
//        buttonState = KEY_PRESSED;
//    else
//        buttonState = NOT_PRESSED;
//}