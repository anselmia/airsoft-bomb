#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <U8glib.h>
#include <Arduino.h>
#include "button.h"
#include "defuse_wire.h"
#include "mode_wire.h"
#include "key.h"
#include "menu.h"
#include "bomb.h"
#include "mode_code.h"

const int pin_plant = 13;

// Input wires
const int wire_pin1 = 15;
const int wire_pin2 = 4;
const int wire_pin3 = 16;
const int wire_pin4 = 17;
const int wire_pin5 = 5;
const int wire_pin6 = 18;
const int wire_pin7 = 19;
const int wire_pin8 = 23;

DEFUSE_WIRE wires[8] = {DEFUSE_WIRE(wire_pin1, 1), DEFUSE_WIRE(wire_pin2, 2), DEFUSE_WIRE(wire_pin3, 3), DEFUSE_WIRE(wire_pin4, 4), DEFUSE_WIRE(wire_pin5, 5), DEFUSE_WIRE(wire_pin6, 6), DEFUSE_WIRE(wire_pin7, 7), DEFUSE_WIRE(wire_pin8, 8)};

// Buttons
KEY keys[16] = {
    KEY(0),
    KEY(1),
    KEY(2),
    KEY(3),
    KEY(4),
    KEY(5),
    KEY(6),
    KEY(7),
    KEY(8),
    KEY(9),
    KEY(10),
    KEY(11),
    KEY(12),
    KEY(13),
    KEY(14),
    KEY(15),
};

BUTTON button_plant = BUTTON(pin_plant);

// 128*64 I2C Screen
U8GLIB_ST7920_128X64_4X u8g(13, 11, 10);

// Menu
MENU menu = MENU();

// Bomb
BOMB bomb = BOMB();

// Game Mode
MODE_WIRE mode_wire = MODE_WIRE(wires);

// Game Mode
MODE_CODE mode_code = MODE_CODE();

// Other
char buf[10];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  u8g.setColorIndex(1);

  DDRC |= _BV(2) | _BV(3); // POWER:Vcc Gnd
  PORTC |= _BV(3);         // VCC PINC3

  u8g.setCursorFont(u8g_font_cursor);
  u8g.setCursorStyle(144);
  delay(100);
}

void select_game_mode()
{
  u8g.drawStr(30, 11, F("MODE DE JEU"));
  u8g.drawStr(30, 33, F("Fil"));
  u8g.drawStr(30, 44, F("Code"));
}

void draw_cursor()
{
  u8g.enableCursor();
  if (menu.actualLine == 0)
    u8g.setCursorPos(12, 33 + 5);
  else
    u8g.setCursorPos(12, 44 + 5);
}

void wire_mode()
{
  if (bomb.state == PLANTED)
  {
    if (mode_wire.boom == false && mode_wire.defused == false)
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
        mode_wire.boom = true;
        bomb.state = EXPLODED;
      }
      if (mode_wire.defused == true)
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
    if (mode_code.boom == false && mode_code.defused == false)
    {
      if (bomb.bombe_code[menu.cursorPos] == bomb.input_code[menu.cursorPos])
      {
        menu.cursorPos += 1;
      }
      else
      {
        if (bomb.input_try == true)
          menu.timer.time_penalty(2);
      }
      bomb.input_try = false;
      if (menu.cursorPos == 4)
      {
        bomb.state = DEFUSED;
      }
      if (menu.timer.mins == 0 && menu.timer.secs == 0)
      {
        mode_code.boom = true;
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
    u8g.drawStr(34, 11, F("TIMER"));
    sprintf(buf, "%d:%d", menu.timer.mins, menu.timer.secs);
    u8g.drawStr(34, 33, buf);
    break;
  case 2:
    u8g.drawStr(30, 11, F("BOMBE"));
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g.drawStr(20, 33, F("...WAITING..."));
      break;
    case ONGOING:
      u8g.drawStr(20, 33, F("...PLANTING..."));
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g.drawStr(20, 33, buf);
      break;
    case DEFUSED:
      u8g.drawStr(20, 33, F("DEFUSED"));
      break;
    case EXPLODED:
      u8g.drawStr(20, 33, F("BOOOOOOOOOOOOM"));
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
    u8g.drawStr(60, 11, F("CODE"));
    sprintf(buf, "%d %d %d %d", bomb.bombe_code[0], bomb.bombe_code[1], bomb.bombe_code[2], bomb.bombe_code[3]);
    u8g.drawStr(30, 33, buf);
    break;
  case 1:
    u8g.drawStr(34, 11, F("TIMER"));
    sprintf(buf, "%d:%d", menu.timer.mins, menu.timer.secs);
    u8g.drawStr(34, 33, buf);
    break;
  case 2:
    u8g.drawStr(30, 11, F("BOMBE"));
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g.drawStr(20, 33, F("...WAITING..."));
      break;
    case ONGOING:
      u8g.drawStr(20, 33, F("...PLANTING..."));
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g.drawStr(20, 33, buf);
      sprintf(buf, "%d %d %d %d", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g.drawStr(20, 55, buf);
      break;
    case DEFUSED:
      u8g.drawStr(20, 33, F("DEFUSED"));
      break;
    case EXPLODED:
      u8g.drawStr(20, 33, F("BOOOOOOOOOOOOM"));
      break;
    }
    break;
  }
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
  for (int i = 0; i < 16; i++)
  {
    keys[i].readButton();
    if (keys[i].buttonState == PRESSED)
    {
      menu.select_action(keys[i].key, bomb);
      break;
    }
  }

  button_plant.readButton();
  if (button_plant.buttonState == PRESSED && menu.actualScreen == 2 && (bomb.state == UNPLANTED || bomb.state == ONGOING))
    bomb.plant();
  else
    bomb.planting_sec = 0;

  u8g.firstPage(); // Select the first memory page of the scrren
  do
  {
    print_screen();
  } while (u8g.nextPage()); // Select the next page
}