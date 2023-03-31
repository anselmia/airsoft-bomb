#include "keys.h"

KEYS::KEYS(int k)
{
    key = k;
    keyState = NOT_PRESSED;
}

// Capa
// void KEYS::readKey()
//{
//    ttp229.readKeys();
//    if (key == ttp229.getKey())
//    {
//        switch (buttonState)
//        {
//        case NOT_PRESSED:
//            buttonState = PUSHED;
//        case PUSHED:
//            buttonState = KEY_PRESSED;
//        }
//    }
//    else
//        buttonState = NOT_PRESSED;
//}

// 4x4
void KEYS::readKey(int k)
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