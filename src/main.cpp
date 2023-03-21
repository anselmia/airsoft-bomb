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

// const int pin_plant = 13;
const int pin_plant = 2; // to remove

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

BUTTON button_plant = BUTTON(pin_plant);

// 128*64 I2C Screen
U8GLIB_ST7920_128X64_4X u8g(13, 11, 10); // Arduino
// U8GLIB_ST7920_128X64_4X u8g(13, 11, 10); // ESP 32 ?

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
  Serial.begin(9600);
  u8g.setColorIndex(1);
  u8g.setCursorFont(u8g_font_cursor);
  u8g.setCursorStyle(144);
  u8g.setFont(u8g_font_tpss);
  delay(100);
}

void draw_cursor()
{
  u8g.enableCursor();
  if (menu.actualLine == 0)
    u8g.setCursorPos(12, 22 + 5);
  else
    u8g.setCursorPos(12, 33 + 5);
}

void select_game_mode()
{
  u8g.drawStr(30, 11, F("MODE DE JEU"));
  u8g.drawStr(30, 33, F("Fil"));
  u8g.drawStr(30, 44, F("Code"));

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
  u8g.disableCursor();
  switch (menu.actualScreen)
  {
  case 1:
    u8g.drawStr(50, 11, F("TIMER"));
    sprintf(buf, "%d:%d", menu.timer.mins, menu.timer.secs);
    u8g.drawStr(48, 33, buf);
    break;
  case 2:
    u8g.drawStr(50, 11, F("BOMBE"));
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g.drawStr(40, 33, F("...WAITING..."));
      break;
    case ONGOING:
      u8g.drawStr(40, 33, F("...PLANTING..."));
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g.drawStr(40, 33, buf);
      break;
    case DEFUSED:
      u8g.drawStr(42, 33, F("DEFUSED"));
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
  u8g.disableCursor();
  switch (menu.actualScreen)
  {
  case 3:
    u8g.drawStr(50, 11, F("CODE"));
    sprintf(buf, "%c %c %c %c", bomb.bombe_code[0], bomb.bombe_code[1], bomb.bombe_code[2], bomb.bombe_code[3]);
    u8g.drawStr(43, 33, buf);
    break;
  case 1:
    u8g.drawStr(50, 11, F("TIMER"));
    sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
    u8g.drawStr(50, 33, buf);
    break;
  case 2:
    u8g.drawStr(50, 11, F("BOMBE"));
    switch (bomb.state)
    {
    case UNPLANTED:
      u8g.drawStr(33, 33, F("... WAITING ..."));
      break;
    case ONGOING:
      u8g.drawStr(30, 33, F("... PLANTING ..."));
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      u8g.drawStr(50, 33, buf);
      sprintf(buf, "%c %c %c %c", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g.drawStr(46, 55, buf);
      break;
    case DEFUSED:
      sprintf(buf, "%c %c %c %c", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g.drawStr(46, 55, buf);
      u8g.drawStr(44, 33, F("DEFUSED"));
      break;
    case EXPLODED:
      sprintf(buf, "%c %c %c %c", bomb.input_code[0], bomb.input_code[1], bomb.input_code[2], bomb.input_code[3]);
      u8g.drawStr(46, 55, buf);
      u8g.drawStr(20, 33, F("BOOOOOOOOOOOOM"));
      break;
    }
    break;
  }
}

void print_progress()
{
  Serial.println(bomb.plantmillis);
  Serial.println(millis());
  u8g.drawBox(3, 40, map(millis(), bomb.plantmillis, bomb.plantmillis + 10000, 0, 125), 10);
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
  int selectedkey = Serial.read();
  for (int i = 0; i < 16; i++)
  {
    keys[i].readButton(selectedkey);
    if (keys[i].buttonState == PRESSED)
    {
      menu.select_action(keys[i].key, bomb);
      break;
    }
  }

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

  u8g.firstPage(); // Select the first memory page of the scrren
  do
  {
    print_screen();
  } while (u8g.nextPage()); // Select the next page
}