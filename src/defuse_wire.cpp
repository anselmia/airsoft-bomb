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
    wireState = DISCONNECTED;
    pinMode(pin, INPUT_PULLUP);

    
}

void DEFUSE_WIRE::readWire()
{
    int pinState = digitalRead(wirePin);
    switch (wireState)
    {
    case DISCONNECTED:
        if (pinState == 0)
            wireState = PLUGGED;

        break;
    case PLUGGED:
        if (pinState == 0)
            wireState = CONNECTED;
        else
            wireState = DISCONNECTED;
        break;
    case CONNECTED:
        if (pinState == 1)
            wireState = DISCONNECTED;
    }
}