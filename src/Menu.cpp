#include "menu.h"

MENU::MENU()
{
    game_mode = 0;
    actualScreen = 0;
    actualLine = 0;
    cursorPos = 0;
    reset = false;
}

void MENU::select_action(int key, BOMB &bomb)
{
    switch (actualScreen)
    {
    case 0:
        switch (key)
        {
        case key_2:
        case key_8:
            if (actualLine == 0)
                actualLine = 1;
            else
                actualLine = 0;
            break;
        case key_star:
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
        case key_2:
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
        case key_8:
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
        case key_4:
        case key_6:
            if (cursorPos == 0)
                cursorPos = 1;
            else
                cursorPos = 0;
            break;
        case key_star:
            actualScreen = 2;
            cursorPos = 0;
            break;
        case key_hash:
            switch (game_mode)
            {
            case 1:
                actualScreen = 0;
                cursorPos = 0;
                reset = true;
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
        case key_0:
        case key_1:
        case key_2:
        case key_3:
        case key_4:
        case key_5:
        case key_6:
        case key_7:
        case key_8:
        case key_9:
        case key_A:
        case key_B:
        case key_C:
        case key_D:
            if (game_mode == 2)
            {
                if (key != bomb.input_code[cursorPos])
                {
                    bomb.input_code[cursorPos] = key;
                    bomb.input_try = true;
                }
            }
            break;
        case key_hash:            
            reset = true;
            break;
        }
        break;
    case 3:
        switch (key)
        {
        case key_0:
        case key_1:
        case key_2:
        case key_3:
        case key_4:
        case key_5:
        case key_6:
        case key_7:
        case key_8:
        case key_9:
        case key_A:
        case key_B:
        case key_C:
            bomb.bombe_code[cursorPos] = key;
            cursorPos++;
            break;
        case key_star:
            actualScreen = 1;
            cursorPos = 0;
            break;
        case key_hash:
            reset = true;
        }
        break;
    }
}
