#include "mode_wire.h"

// Before to start verify all wire coonected
MODE_WIRE::MODE_WIRE(DEFUSE_WIRE (&wires)[8])
{
    // Init randomly usage for each wire
    int newitem = -1;
    for (int i = 0; i < 8; i++)
    {
        bool unique;
        do
        {
            unique = true;
            newitem = random(8);
            for (int i1 = 0; i1 < i; i1++)
            {
                if (wires[i1].usage == newitem)
                {
                    unique = false;
                    break;
                }
            }
        } while (!unique);
        wires[i].usage = newitem;
    }

    wires_diffused = 0;
    diffused = false;
    boom = false;
}

void MODE_WIRE::disconect_cable(DEFUSE_WIRE wire , TIMER &timer)
{
    get_wire_usage(wire);
    switch (action)
          {
          case DIFFUSE:
            Diffuse();
            break;
          case PENALTY:
            timer.penalty();
            break;
          case BOOM:
            timer.boom();
            break;
          }
}

void MODE_WIRE::get_wire_usage(DEFUSE_WIRE wire)
{
    action = -1;
    if (wire.usage == dif_wire[0])
        action = DIFFUSE;
    else if (wire.usage == dif_wire[1])
        action = DIFFUSE;
    else if (wire.usage == dif_wire[2])
        action = DIFFUSE;
    else if (wire.usage == dif_wire[3])
        action = DIFFUSE;
    else if (wire.usage == time_penalty[0])
        action = PENALTY;
    else if (wire.usage == time_penalty[1])
        action = PENALTY;
    else if (wire.usage == time_penalty[2])
        action = PENALTY;
    else if (wire.usage == boom_wire)
        action = BOOM;
}

void MODE_WIRE::Diffuse()
{
    wires_diffused++;
    if (wires_diffused == 4)
        diffused = true;
}