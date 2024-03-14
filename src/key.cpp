#include "key.h"

KEY::KEY(int k)
{
    key = k;
    buttonState = NOT_PRESSED;
}

// to remove
// void KEY::readButton(int selectedkey)
//{
//    if (key == selectedkey)
//        buttonState = PRESSED;
//    else
//        buttonState = NOT_PRESSED;
//}