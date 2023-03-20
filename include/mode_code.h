#ifndef MODE_CODE_h
#define MODE_CODE_h
#include <Arduino.h>

#include "timer.h"

class MODE_CODE
{
private:
public:
    MODE_CODE();
    bool boom;
    int action;
    bool diffused;
    void Diffuse();
};
#endif