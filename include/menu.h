#ifndef MENU_h
#define MENU_h
#include "timer.h"
#include "Arduino.h"
#include "key.h"
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
    char bombe_code[5] = "    ";
    char input_code[5] = "    ";
    void select_action(int key);
    int cursorPos;
    void reset_game();
};
#endif