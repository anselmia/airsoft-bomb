#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "button.h"
#include "defuse_wire.h"
#include "mode_wire.h"
#include "timer.h"

// Usable pin for further use
// const int pinwait = 13;

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

MODE_WIRE mode_wire = MODE_WIRE(wires);
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char buf[10];
TIMER timer = TIMER(30, 5);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  delay(100);
}

void loop()
{
  if (mode_wire.boom == false || mode_wire.diffused == true)
  {
    timer.updateTime();
    display.clearDisplay();
    display.setCursor(0, 0);
    sprintf(buf, "%d:%d:%d", wires[0].num, wires[0].used, wires[0].usage);
    display.print(buf);
    display.setCursor(64, 0);
    sprintf(buf, " %02d:%02d", timer.mins, timer.secs);
    display.print(buf);
    display.println(F(""));
    sprintf(buf, "%d:%d:%d", wires[1].num, wires[1].used, wires[1].usage);
    display.print(buf);
    for (int i = 0; i < 8; i++)
    {
      wires[i].readWire();
      if (wires[i].wireState == DISCONNECTED && wires[i].used == false)
      {
        wires[i].used = true;
        mode_wire.get_wire_usage(wires[i]);
        display.setCursor(64, 8);
        switch (mode_wire.action)
        {
        case DIFFUSE:
          display.print("dif");
          mode_wire.Diffuse();
          break;
        case PENALTY:
          display.print("pen");
          timer.penalty();
          break;
        case BOOM:
          display.println("boom");
          timer.boom();
          break;
        }
        delay(2000);
        break;
      }
    }
    display.println(F(""));
    sprintf(buf, "%d:%d:%d", wires[2].num, wires[2].used, wires[2].usage);
    display.print(buf);

    if (timer.mins == 0 && timer.secs == 0)
    {
      mode_wire.boom = true;
      display.setCursor(64, 16);
      display.print("BOOM");
    }
    display.println(F(""));

    sprintf(buf, "%d:%d:%d", wires[3].num, wires[3].used, wires[3].usage);
    display.print(buf);
    if (mode_wire.diffused == true)
    {
      display.setCursor(64, 24);
      display.print("DIF");
    }
    display.println(F(""));
    for (int i = 4; i < 8; i++)
    {
      sprintf(buf, "%d:%d:%d", wires[i].num, wires[i].used, wires[i].usage);
      display.println(buf);
    }
    display.display();
  }
}

// to test input
// for (int i = 0; i < 8; i++)
// {
//   wires[i].readWire();
//   sprintf(buf, "wire %d: %d", wires[i].num, wires[i].usage);
//   display.println(buf);
// }