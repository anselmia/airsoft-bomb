#include <SPI.h>
#include "TTP229.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>
#include "button.h"
#include "defuse_wire.h"
#include "mode_wire.h"
#include "menu.h"
// #include "led.h"
// #include <I2CKeyPad.h>
// esp
// const int pin_plant = 22; // External pullup 51K
// const int pin_key = 21;   // External pullup 51K
// const int pin_buzzer = 16;
// Arduino
const int pin_plant = 6; // External pullup 51K
const int pin_key = 7;   // External pullup 51K
const int pin_buzzer = 5;

// Input wires ESP32
// const int wire_pin1 = 16;
// const int wire_pin2 = 26;
// const int wire_pin3 = 27;
// const int wire_pin4 = 17;
// arduino
const int wire_pin1 = 0;
const int wire_pin2 = 1;
const int wire_pin3 = 2;
const int wire_pin4 = 3;

// Keypad
// I2CKeyPad keyPad(I2CADDR);

// Defuse wire
DEFUSE_WIRE wires[8] = {
    DEFUSE_WIRE(wire_pin1, 1),
    DEFUSE_WIRE(wire_pin1, 2),
    DEFUSE_WIRE(wire_pin2, 3),
    DEFUSE_WIRE(wire_pin2, 4),
    DEFUSE_WIRE(wire_pin3, 5),
    DEFUSE_WIRE(wire_pin3, 6),
    DEFUSE_WIRE(wire_pin4, 7),
    DEFUSE_WIRE(wire_pin4, 8)};

// Keys
KEYS keys[16] = {
    KEYS(key_0),
    KEYS(key_1),
    KEYS(key_2),
    KEYS(key_3),
    KEYS(key_4),
    KEYS(key_5),
    KEYS(key_6),
    KEYS(key_7),
    KEYS(key_8),
    KEYS(key_9),
    KEYS(key_A),
    KEYS(key_B),
    KEYS(key_C),
    KEYS(key_D),
    KEYS(key_star),
    KEYS(key_hash),
};

// Keypad ESP
// const int SCL_PIN = 12;
// const int SDO_PIN = 13;
// Keypad arduino
const int SCL_PIN = 3;
const int SDO_PIN = 4;
TTP229 ttp229 = TTP229(SCL_PIN, SDO_PIN);

// Buttons
BUTTON button_plant = BUTTON(pin_plant);
BUTTON button_arm = BUTTON(pin_key);

// 128*64 I2C Screen
// U8GLIB_ST7920_128X64_4X u8g(13, 11, 10); // Arduino
// U8GLIB_ST7920_128X64_4X u8g(13, 11, 10); // ESP 32 ?
// U8G2_ST7565_JLX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/18, /* data=*/23, /* cs=*/25, /* dc=*/33, /* reset=*/32); /// good esp32
U8G2_ST7565_JLX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8); /// good
// Menu
MENU menu = MENU();

// Bomb
BOMB bomb = BOMB();

// Game Mode
MODE_WIRE mode_wire = MODE_WIRE(wires);
uint8_t defused = 0;

// Other
char buf[10];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  // with 4x4 keypad
  // if (keyPad.begin() == false)
  //{
  //   Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
  //   while (1)
  //     ;
  // }
  //
  // keyPad.loadKeyMap(layout_keypad);
  u8g2.begin();
  u8g2.setColorIndex(1);
  u8g2.setFont(u8g2_font_5x7_tr);

  delay(100);
}

void draw_cursor()
{
  if (menu.actualLine == 0)
    u8g2.drawBox(12, 22, 5, 7);
  else
    u8g2.drawBox(12, 33, 5, 7);
}

void select_game_mode()
{
  if (bomb.state == ARMED)
    u8g2.drawStr(20, 11, " !!! BOMBE ARMEE !!!");
  else
  {
    u8g2.drawStr(30, 11, "MODE DE JEU");
    u8g2.drawStr(30, 33, "Fil");
    u8g2.drawStr(30, 44, "Code");

    draw_cursor();
  }
}

void wire_mode()
{
  Serial.println(bomb.state);
  Serial.println(mode_wire.wires_defuse);
  if (bomb.state == PLANTED)
  {
    if (bomb.boom == false && bomb.defused == false)
    {
      for (int i = 0; i < 8; i++)
      {
        wires[i].readWire();
        if (wires[i].wireState == DISCONNECTED && wires[i].used == false)
        {
          wires[i].used = true;
          mode_wire.disconnect_cable(wires[i], menu.timer);
        }
      }
      if (menu.timer.mins == 0 && menu.timer.secs == 0)
      {
        bomb.boom = true;
        bomb.state = EXPLODED;
      }

      if (mode_wire.wires_defuse == 4)
      {
        bomb.state = DEFUSED;
      }
    }
    menu.timer.updateTime();
  }
}

void code_mode()
{
  if (bomb.state == PLANTED)
  {
    if (bomb.boom == false && bomb.defused == false)
    {
      if (bomb.bombe_code[menu.cursorPos] == bomb.input_code[menu.cursorPos])
      {
        menu.cursorPos += 1;
      }
      else
      {
        if (bomb.input_try == true)
          menu.timer.time_penalty(4);
      }
      bomb.input_try = false;
      if (menu.cursorPos == 4)
      {
        bomb.state = DEFUSED;
      }
      if (menu.timer.mins == 0 && menu.timer.secs == 0)
      {
        bomb.boom = true;
        bomb.state = EXPLODED;
      }
      menu.timer.updateTime();
    }
  }
}

void wire_mode_screen()
{
  switch (menu.actualScreen)
  {
  case 1:
    u8g2.drawStr(50, 11, "TIMER");
    sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
    u8g2.drawStr(48, 33, buf);
    break;
  case 2:
    u8g2.drawStr(50, 11, "BOMBE");
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g2.drawStr(40, 33, "...NOT ARMED...");
      break;
    case ARMED:
      u8g2.drawStr(40, 33, "...WAITING...");
      break;
    case ONGOING:
      u8g2.drawStr(30, 33, "...PLANTING...");
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g2.drawStr(40, 33, buf);
      char wire_state[7];
      for (int i = 0; i < 8; i++)
      {
        if (wires[i].used == true)
          wire_state[i] = 'V';
        else
          wire_state[i] = 'X';
      }
      sprintf(buf, "%c %c %c %c %c %c %c %c", wire_state[0], wire_state[1], wire_state[2], wire_state[3], wire_state[4], wire_state[5], wire_state[6], wire_state[7]);
      u8g2.drawStr(25, 55, buf);
      break;
    case DEFUSED:
      u8g2.drawStr(42, 33, "DEFUSED");
      break;
    case EXPLODED:
      u8g2.drawStr(20, 33, "BOOOOOOOOOOOOM");
      break;
    }
    break;
  }
}

void code_mode_screen()
{
  switch (menu.actualScreen)
  {
  case 3:
    u8g2.drawStr(50, 11, "CODE");
    sprintf(buf, "%d %d %d %d", bomb.bombe_code[0], bomb.bombe_code[1], bomb.bombe_code[2], bomb.bombe_code[3]);
    u8g2.drawStr(43, 33, buf);
    break;
  case 1:
    u8g2.drawStr(50, 11, "TIMER");
    sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
    u8g2.drawStr(50, 33, buf);
    break;
  case 2:
    u8g2.drawStr(50, 11, "BOMBE");
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g2.drawStr(33, 33, "... NOT ARMED ...");
      break;
    case ARMED:
      u8g2.drawStr(33, 33, "... WAITING ...");
      break;
    case ONGOING:
      u8g2.drawStr(30, 33, "... PLANTING ...");
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g2.drawStr(50, 33, buf);
      sprintf(buf, "%d %d %d %d", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g2.drawStr(46, 55, buf);
      break;
    case DEFUSED:
      sprintf(buf, "%d %d %d %d", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g2.drawStr(46, 55, buf);
      u8g2.drawStr(44, 33, "DEFUSED");
      break;
    case EXPLODED:
      sprintf(buf, "%d %d %d %d", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g2.drawStr(46, 55, buf);
      u8g2.drawStr(20, 33, "BOOOOOOOOOOOOM");
      break;
    }
    break;
  }
}

void print_progress()
{
  // Serial.println(bomb.plantmillis);
  // Serial.println(millis());
  u8g2.drawBox(3, 40, map(millis(), bomb.plantmillis, bomb.plantmillis + 10000, 0, 125), 10);
}

void print_screen()
{
  switch (menu.game_mode)
  {
  case 0:
    select_game_mode();
    break;
  case 1:
    wire_mode();
    wire_mode_screen();
    break;
  case 2:
    code_mode();
    code_mode_screen();
    break;
  }
}

void readButton(bool action)
{
  button_arm.readButton();
  if (action == false)
  {
    if (bomb.state <= ARMED && button_arm.buttonState == KEY_PRESSED)
      bomb.state = ARMED;
    if (button_arm.buttonState != KEY_PRESSED)
      bomb.state = UNPLANTED;
  }

  button_plant.readButton();
  if (button_plant.buttonState == KEY_PRESSED && (bomb.state == ARMED || bomb.state == ONGOING) && menu.actualScreen == 2 && bomb.defused == false && bomb.boom == false)
  {
    bomb.plant();
    print_progress();
  }
  else
  {
    if (bomb.state == ONGOING && bomb.defused == false && bomb.boom == false)
    {
      bomb.planting_sec = 0;
      bomb.state = UNPLANTED;
    }
  }
}

void reset_game()
{
  menu.reset = false;
  bomb.state = UNPLANTED;
  bomb.boom = false;
  bomb.defused = false;
  bomb.planting_sec = 0;
  bomb.plantmillis = 0;
  menu.actualScreen = 0;
  menu.cursorPos = 0;
  menu.game_mode = 0;
  menu.timer.mins = 20;
  menu.timer.secs = 0;
  menu.actualLine = 0;
  bomb.bombe_code[0] = 0;
  bomb.bombe_code[1] = 0;
  bomb.bombe_code[2] = 0;
  bomb.bombe_code[3] = 0;
  bomb.input_code[0] = 0;
  bomb.input_code[1] = 0;
  bomb.input_code[2] = 0;
  bomb.input_code[3] = 0;
  mode_wire.wires_defuse = 0;
  for (int i = 0; i < 8; i++)
    wires[i].used = false;
}

void loop()
{
  Serial.println(bomb.state);
  Serial.println(mode_wire.wires_defuse);
  bool action = false;
  uint8_t key = ttp229.GetKey16(); // to remove
  // int key = keyPad.getChar();
  for (int i = 0; i < 16; i++)
  {
    if (keys[i].key == key)
    {
      menu.select_action(keys[i].key, bomb);
      action = true;
      break;
    }
  }
  ttp229.set_Key16_to_0();

  readButton(action);

  u8g2.firstPage(); // Select the first memory page of the scrren
  do
  {
    print_screen();
  } while (u8g2.nextPage()); // Select the next page

  if (menu.reset == true)
    reset_game();
}