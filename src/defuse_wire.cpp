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
    float pinState = (analogRead(wirePin) * 3.3) / 4096;
    Serial.println(pinState);
    if (num % 2 == 0)
    {
        switch (wireState)
        {
        case DISCONNECTED:
            if ((pinState <= 0.9 && pinState >= 0.6) || (pinState <= 1.8 && pinState >= 1.5))
                wireState = PLUGGED;

            break;
        case PLUGGED:
            if (pinState < 0.3 || (pinState <= 1.4 && pinState >= 1.1))
                wireState = CONNECTED;
            else
                wireState = DISCONNECTED;
            break;
        case CONNECTED:
            if (pinState < 0.3 || (pinState <= 1.4 && pinState >= 1.1))
                wireState = DISCONNECTED;
        }
    }
    else
    {
        switch (wireState)
        {
        case DISCONNECTED:
            if ((pinState <= 1.4 && pinState >= 1.1) || (pinState <= 1.8 && pinState >= 1.5))
                wireState = PLUGGED;
            break;
        case PLUGGED:
            if (pinState < 0.3 || (pinState <= 0.9 && pinState >= 0.6))
                wireState = CONNECTED;
            else
                wireState = DISCONNECTED;
            break;
        case CONNECTED:
            if (pinState < 0.3 || (pinState <= 0.9 && pinState >= 0.6))
                wireState = DISCONNECTED;
        }
    }
}