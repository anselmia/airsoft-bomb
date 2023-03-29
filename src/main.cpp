#include <SPI.h>
#include "TTP229.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>
#include "button.h"
#include "defuse_wire.h"
#include "mode_wire.h"
#include "key.h"
#include "menu.h"
#include "bomb.h"

// const int pin_plant = 13;
const int pin_plant = 16; // to remove

// Input wires ESP32
const int wire_pin1 = 14;
const int wire_pin2 = 27;
const int wire_pin3 = 15;
const int wire_pin4 = 17;
const int wire_pin5 = 19;
const int wire_pin6 = 35;
const int wire_pin7 = 34;
const int wire_pin8 = 26;

DEFUSE_WIRE wires[8] = {
    DEFUSE_WIRE(wire_pin1, 1),
    DEFUSE_WIRE(wire_pin2, 2),
    DEFUSE_WIRE(wire_pin3, 3),
    DEFUSE_WIRE(wire_pin4, 4),
    DEFUSE_WIRE(wire_pin5, 5),
    DEFUSE_WIRE(wire_pin6, 6),
    DEFUSE_WIRE(wire_pin7, 7),
    DEFUSE_WIRE(wire_pin8, 8)};

// Buttons
KEY keys[16] = {
    KEY(key_0),
    KEY(key_1),
    KEY(key_2),
    KEY(key_3),
    KEY(key_4),
    KEY(key_5),
    KEY(key_6),
    KEY(key_7),
    KEY(key_8),
    KEY(key_9),
    KEY(key_A),
    KEY(key_B),
    KEY(key_C),
    KEY(key_D),
    KEY(key_star),
    KEY(key_hash),
};

// Keypad
const int SCL_PIN = 12;
const int SDO_PIN = 13;
TTP229 ttp229 = TTP229(SCL_PIN, SDO_PIN);

BUTTON button_plant = BUTTON(pin_plant);

// 128*64 I2C Screen
// U8GLIB_ST7920_128X64_4X u8g(13, 11, 10); // Arduino
// U8GLIB_ST7920_128X64_4X u8g(13, 11, 10); // ESP 32 ?
U8G2_ST7565_JLX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/18, /* data=*/23, /* cs=*/25, /* dc=*/33, /* reset=*/32); /// good

// Menu
MENU menu = MENU();

// Bomb
BOMB bomb = BOMB();

// Game Mode
MODE_WIRE mode_wire = MODE_WIRE(wires);

// Other
char buf[10];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  u8g2.begin();
  u8g2.setColorIndex(1);
  // u8g2.setCursorFont(u8g_font_cursor);
  // u8g2.setCursorStyle(144);
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
  u8g2.drawStr(30, 11, "MODE DE JEU");
  u8g2.drawStr(30, 33, "Fil");
  u8g2.drawStr(30, 44, "Code");

  draw_cursor();
}

void wire_mode()
{
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
        mode_wire.wires_defused = 0;
      }
      if (mode_wire.wires_defused == 4)
      {
        bomb.state = DEFUSED;
        mode_wire.wires_defused = 0;
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
    sprintf(buf, "%d:%d", menu.timer.mins, menu.timer.secs);
    u8g2.drawStr(48, 33, buf);
    break;
  case 2:
    u8g2.drawStr(50, 11, "BOMBE");
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g2.drawStr(40, 33, "...WAITING...");
      break;
    case ONGOING:
      u8g2.drawStr(40, 33, "...PLANTING...");
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g2.drawStr(40, 33, buf);
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
  Serial.println(bomb.plantmillis);
  Serial.println(millis());
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

void loop()
{
  uint8_t key = ttp229.GetKey16(); // Non Blocking
  for (int i = 0; i < 16; i++)
  {
    if (keys[i].key == key)
    {
      menu.select_action(keys[i].key, bomb);
      break;
    }
  }
  ttp229.set_Key16_to_0();

  button_plant.readButton();
  if (button_plant.buttonState == PRESSED && menu.actualScreen == 2 && (bomb.state == UNPLANTED || bomb.state == ONGOING) && bomb.defused == false && bomb.boom == false)
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

  u8g2.firstPage(); // Select the first memory page of the scrren
  do
  {
    print_screen();
  } while (u8g2.nextPage()); // Select the next page
}