#include "defuse_wire.h"
DEFUSE_WIRE::DEFUSE_WIRE()
{
}

DEFUSE_WIRE::DEFUSE_WIRE(int pin, int num_wire)
{
    usage = -1;
    wirePin = pin;
    num = num_wire;
    used = false;
    wireState = CONNECTED;
}

void DEFUSE_WIRE::readWire()
{
    // float pinState = (analogRead(wirePin) * 3.3) / 4096; ESP
    float pinState = (analogRead(wirePin) * 3.3) / 1024; // Arduino

    // ESP
    // if (num % 2 == 0)
    // {
    //    switch (wireState)
    //    {
    //    case DISCONNECTED:
    //        if (pinState < 0.3 || (pinState <= 1.7 && pinState >= 1.4))
    //            wireState = PLUGGED;
    //        break;
    //    case PLUGGED:
    //        if (pinState < 0.3 || (pinState <= 1.7 && pinState >= 1.4))
    //            wireState = CONNECTED;
    //        else if ((pinState <= 2.4 && pinState >= 2.2) || (pinState <= 2.1 && pinState >= 1.8))
    //            wireState = DISCONNECTED;
    //        break;
    //    case CONNECTED:
    //        if ((pinState <= 2.4 && pinState >= 2.2) || (pinState <= 2.1 && pinState >= 1.8))
    //            wireState = DISCONNECTED;
    //    }
    // }
    // else
    // {
    //    switch (wireState)
    //    {
    //    case DISCONNECTED:
    //        if (pinState < 0.3 || (pinState <= 2.1 && pinState >= 1.8))
    //            wireState = PLUGGED;
    //        break;
    //    case PLUGGED:
    //        if (pinState < 0.3 || (pinState <= 2.1 && pinState >= 1.8))
    //            wireState = CONNECTED;
    //        else if ((pinState <= 2.4 && pinState >= 2.2) || (pinState <= 1.7 && pinState >= 1.4))
    //            wireState = DISCONNECTED;
    //        break;
    //    case CONNECTED:
    //        if ((pinState <= 2.4 && pinState >= 2.2) || (pinState <= 1.7 && pinState >= 1.4))
    //            wireState = DISCONNECTED;
    //    }
    // }

    // Arduino
    if (num % 2 == 0)
    {
        switch (wireState)
        {
        case DISCONNECTED:
            if (pinState < 0.3 || (pinState <= 1.5 && pinState >= 1))
                wireState = PLUGGED;
            break;
        case PLUGGED:
            if (pinState < 0.3 || (pinState <= 1.5 && pinState >= 1))
                wireState = CONNECTED;
            else if ((pinState <= 1.8 && pinState >= 1.6) || (pinState <= 2.4 && pinState >= 1.9))
                wireState = DISCONNECTED;
            break;
        case CONNECTED:
            if ((pinState <= 1.8 && pinState >= 1.6) || (pinState <= 2.4 && pinState >= 1.9))
                wireState = DISCONNECTED;
        }
    }
    else
    {
        switch (wireState)
        {
        case DISCONNECTED:
            if (pinState < 0.3 || (pinState <= 1.8 && pinState >= 1.6))
                wireState = PLUGGED;
            break;
        case PLUGGED:
            if (pinState < 0.3 || (pinState <= 1.8 && pinState >= 1.6))
                wireState = CONNECTED;
            else if ((pinState <= 1.5 && pinState >= 1) || (pinState <= 2.4 && pinState >= 1.9))
                wireState = DISCONNECTED;
            break;
        case CONNECTED:
            if ((pinState <= 1.5 && pinState >= 1) || (pinState <= 2.4 && pinState >= 1.9))
                wireState = DISCONNECTED;
        }
    }
}