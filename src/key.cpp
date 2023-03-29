#include "key.h"

KEY::KEY(int k)
{
    key = k;
    buttonState = NOT_PRESSED;
}

// to uncomment with keyboard
// void KEY::readButton()
//{
//    ttp229.readKeys();
//    if (key == ttp229.getKey())
//    {
//        switch (buttonState)
//        {
//        case NOT_PRESSED:
//            buttonState = PUSHED;
//        case PUSHED:
//            buttonState = PRESSED;
//        }
//    }
//    else
//        buttonState = NOT_PRESSED;
//}

// to remove
// void KEY::readButton(int selectedkey)
//{
//    if (key == selectedkey)
//        buttonState = PRESSED;
//    else
//        buttonState = NOT_PRESSED;
//}