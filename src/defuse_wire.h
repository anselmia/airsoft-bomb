#ifndef DIFUSE_WIRE_h
#define DIFUSE_WIRE_h
#include <Arduino.h>

static const byte DISCONNECTED = 0;
static const byte PLUGGED = 1;
static const byte CONNECTED = 2;

class DEFUSE_WIRE
{
private:
    int wirePin;

public:
    DEFUSE_WIRE();
    DEFUSE_WIRE(int pin, int num_wire);
    int num;
    int usage; // wire functionality
    bool used;
    byte wireState;
    void readWire();
};

#endif