#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef TTP229_h
#define TTP229_h

class TTP229
{
public:
    TTP229(uint8_t sclPin, uint8_t sdoPin);
    uint8_t
    ReadKey8(),
        GetKey8(),
        ReadKeys8(),
        GetKeys8(),
        ReadKey16(),
        GetKey16();
    void set_Key16_to_0();

    uint16_t
    ReadKeys16(),
        GetKeys16();

private:
    void
    WaitForTouch(),
        Key8(),
        Keys8(),
        Key16(),
        Keys16();
    bool
    IsTouch(),
        GetBit();

    uint8_t _sclPin, _sdoPin;
    uint8_t _key8, _keys8, _key16;
    uint16_t _keys16;
};

#endif
