#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Menu
MENU menu = MENU();

// Bomb
BOMB bomb = BOMB();

// Game Mode
MODE_WIRE mode_wire = MODE_WIRE(wires);
MODE_CODE mode_code = MODE_CODE();

// Other
char buf[10];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
  }

  delay(100);
}

void draw_cursor(int y)
{
  display.drawRect(0, y, 5, 8, SSD1306_WHITE);
}

void select_game_mode_screen()
{
  display.clearDisplay();
  switch (menu.actualScreen)
  {
  case 0:
    display.setCursor(30, 0);
    display.println(F("MODE DE JEU"));
    display.println(F(""));
    display.setCursor(30, 16);
    display.println(F("Fils"));
    display.setCursor(30, 24);
    display.println(F("Code"));
    draw_cursor(menu.actualLine * 8 + 16);
    break;
  }
  display.display();
}

void wire_mode()
{
  if (bomb.state == PLANTED)
  {
    if (mode_wire.boom == false || mode_wire.diffused == true)
    {
      menu.timer.updateTime();
      for (int i = 0; i < 8; i++)
      {
        wires[i].readWire();
        if (wires[i].wireState == DISCONNECTED && wires[i].used == false)
        {
          wires[i].used = true;
          mode_wire.disconect_cable(wires[i], menu.timer);
        }
      }
      if (menu.timer.mins == 0 && menu.timer.secs == 0)
      {
        mode_wire.boom = true;
        bomb.state = EXPLODED;
      }
      if (mode_wire.diffused == true)
      {
        bomb.state = DIFFUSED;
      }
    }
  }
}

void code_mode()
{
  if (bomb.state == PLANTED)
  {
    if (mode_code.boom == false || mode_code.diffused == false)
    {
      menu.timer.updateTime();

      if (menu.input_code[menu.cursorPos] == menu.bombe_code[menu.cursorPos])
      {
        menu.cursorPos++;
      }
      else
      {
        menu.timer.penalty
      }
    }
  }
}

void wire_mode_screen()
{
  display.clearDisplay();
  switch (menu.actualScreen)
  {
  case 1:
    display.setTextSize(2);
    display.setCursor(60, 0);
    display.println(F("TIMER"));
    display.println(F(""));
    display.setCursor(30, 16);
    sprintf(buf, "%d:%d", menu.timer.mins, menu.timer.secs);
    display.print(buf);
    display.setTextSize(1);
    break;
  case 2:
    display.setTextSize(2);
    display.setCursor(40, 0);
    display.println(F("Activation"));
    display.println(F(""));
    display.setCursor(20, 16);
    switch (bomb.state)
    {
    case UNPLANTED:
      display.println(F("...EN ATTENTE..."));
      break;
    case ONGOING:
      display.println(F("...ARMEMENT..."));
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      display.print(buf);
      break;
    case DIFFUSED:
      display.print("Bombe désactivée");
      break;
    case EXPLODED:
      display.print("BOOOOOOOOOOOOM");
      break;
    }
    display.setTextSize(1);
    break;
  }
  display.display();
}

void code_mode_screen()
{
  display.clearDisplay();
  switch (menu.actualScreen)
  {
  case 3:
    display.setTextSize(2);
    display.setCursor(60, 0);
    display.println(F("CODE"));
    display.println(F(""));
    display.setCursor(30, 16);
    sprintf(buf, "%d %d %d %d", menu.bombe_code[0], menu.bombe_code[1], menu.bombe_code[2], menu.bombe_code[3]);
    display.print(buf);
    display.setTextSize(1);
    break;
  case 1:
    display.setTextSize(2);
    display.setCursor(60, 0);
    display.println(F("TIMER"));
    display.println(F(""));
    display.setCursor(30, 16);
    sprintf(buf, "%d:%d", menu.timer.mins, menu.timer.secs);
    display.print(buf);
    display.setTextSize(1);
    break;
  case 2:
    display.setTextSize(2);
    display.setCursor(40, 0);
    display.println(F("Activation"));
    display.println(F(""));
    display.setCursor(20, 16);
    switch (bomb.state)
    {
    case UNPLANTED:
      display.println(F("...EN ATTENTE..."));
      break;
    case ONGOING:
      display.println(F("...ARMEMENT..."));
      break;
    case PLANTED:
      sprintf(buf, "%02d:%02d", menu.timer.mins, menu.timer.secs);
      display.print(buf);
      display.println(F(""));
      sprintf(buf, "%d %d %d %d", menu.input_code[0], menu.input_code[1], menu.input_code[2], menu.input_code[3]);
      display.print(buf);
      break;
    case DIFFUSED:
      display.print("Bombe désactivée");
      break;
    case EXPLODED:
      display.print("BOOOOOOOOOOOOM");
      break;
    }
    display.setTextSize(1);
    break;
  }
  display.display();
}

// for testing; to remove
void select()
{
  byte selectedButton;
  selectedButton = Serial.read();
  Serial.println(selectedButton);
  menu.select_action(selectedButton);
}

void loop()
{
  // for (int i = 0; i < 16; i++)
  //{
  //   keys[i].readButton();
  //   if (keys[i].buttonState == PRESSED)
  //   {
  //     menu.select_action(keys[i].key);
  //     break;
  //   }
  // }
  Serial.println("LOOP");
  select();
  button_plant.readButton();
  if (button_plant.buttonState == PRESSED && menu.actualScreen == 2 && (bomb.state == UNPLANTED || bomb.state == ONGOING))
    bomb.plant();
  else
    bomb.planting_sec = 0;

  switch (menu.game_mode)
  {
  case 0:
    select_game_mode_screen();
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