#ifndef MENU_h
#define MENU_h
#include "timer.h"
#include "Arduino.h"
#include "keys.h"
#include "bomb.h"

class MENU
{
private:
public:
    MENU();
    int game_mode;
    int actualScreen;
    int actualLine;
    TIMER timer = TIMER(0, 20);
    void select_action(int key, BOMB &bomb);
    int cursorPos;
    void reset_game(BOMB &bomb);
};
#endif