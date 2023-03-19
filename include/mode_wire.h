#ifndef MODE_WIRE_h
#define MODE_WIRE_h
#include <Arduino.h>
#include "defuse_wire.h"
#include "timer.h"

static const int dif_wire[4] = {0, 1, 2, 3};
static const int time_penalty[3] = {4, 5, 6};
static const int boom_wire = 7;

static const byte DIFFUSE = 0;
static const byte PENALTY = 1;
static const byte BOOM = 2;

class MODE_WIRE
{
private:
    void get_wire_usage(DEFUSE_WIRE wire);

public:
    MODE_WIRE(DEFUSE_WIRE (&wires)[8]);
    bool boom;
    int action;
    int wires_diffused;
    bool diffused;
    void Diffuse();
    void disconect_cable(DEFUSE_WIRE wire, TIMER &timer);
};

#endif