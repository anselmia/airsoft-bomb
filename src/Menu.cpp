#include "menu.h"

MENU::MENU()
{
    game_mode = 0;
    actualScreen = 0;
    actualLine = 0;
    cursorPos = 0;
}

void MENU::select_action(int key, BOMB &bomb)
{
    switch (actualScreen)
    {
    case 0:
        switch (key)
        {
        case but_2:
        case but_8:
            if (actualLine == 0)
                actualLine = 1;
            else
                actualLine = 0;
            break;
        case but_star:
            switch (actualLine)
            {
            case 0:
                game_mode = 1;
                actualScreen = 1;
                actualLine = 0;
                break;
            case 1:
                game_mode = 2;
                actualScreen = 3;
                actualLine = 0;
                break;
            }
            break;
        }
        break;
    case 1:
        switch (key)
        {
        case but_2:
            switch (cursorPos)
            {
            case 0:
                timer.increase_min();
                break;
            case 1:
                timer.increase_sec();
                break;
            }
            break;
        case but_8:
            switch (cursorPos)
            {
            case 0:
                timer.decrease_min();
                break;
            case 1:
                timer.decrease_sec();
                break;
            }
            break;
        case but_4:
        case but_6:
            if (cursorPos == 0)
                cursorPos = 1;
            else
                cursorPos = 0;
        case but_star:
            actualScreen = 2;
            cursorPos = 0;
            break;
        case but_hash:
            switch (game_mode)
            {
            case 1:
                actualScreen = 0;
                cursorPos = 0;
                reset_game(bomb);
                break;
            case 2:
                actualScreen = 3;
                cursorPos = 0;
                break;
            }
            break;
        }
        break;
    case 2:
        switch (key)
        {
        case but_0:
        case but_1:
        case but_2:
        case but_3:
        case but_4:
        case but_5:
        case but_6:
        case but_7:
        case but_8:
        case but_9:
        case but_A:
        case but_B:
        case but_C:
        case but_D:
            if (game_mode == 2)
            {
                if (key != bomb.input_code[cursorPos])
                {
                    bomb.input_code[cursorPos] = key;
                    bomb.input_try = true;
                }
            }
            break;
        case but_hash:
            actualScreen = 0;
            cursorPos = 0;
            reset_game(bomb);
            break;
        }
        break;
    case 3:
        switch (key)
        {
        case but_0:
        case but_1:
        case but_2:
        case but_3:
        case but_4:
        case but_5:
        case but_6:
        case but_7:
        case but_8:
        case but_9:
        case but_A:
        case but_B:
        case but_C:
            bomb.bombe_code[cursorPos] = key;
            break;
        case but_star:
            actualScreen = 1;
            cursorPos = 0;
            break;
        case but_hash:
            reset_game(bomb);
        }
        break;
    }
}

void MENU::reset_game(BOMB &bomb)
{
    game_mode = 0;
    timer.mins = 20;
    timer.secs = 0;
    bomb.bombe_code[0] = ' ';
    bomb.bombe_code[1] = ' ';
    bomb.bombe_code[2] = ' ';
    bomb.bombe_code[3] = ' ';
    bomb.input_code[0] = ' ';
    bomb.input_code[1] = ' ';
    bomb.input_code[2] = ' ';
    bomb.input_code[3] = ' ';
}