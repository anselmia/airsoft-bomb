#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "button.h"

// Input
const int wire1 = 4;
const int wire2 = 16;
const int wire3 = 17;
const int wire4 = 5;

BUTTON button1 = BUTTON(wire1);
BUTTON button2 = BUTTON(wire2);
BUTTON button3 = BUTTON(wire3);
BUTTON button4 = BUTTON(wire4);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.setTextSize(2);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Define pin for wire
  pinMode(wire1, INPUT_PULLUP);
  pinMode(wire2, INPUT_PULLUP);
  pinMode(wire3, INPUT_PULLUP);
  pinMode(wire4, INPUT_PULLUP);

  delay(100);
}

void loop()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  button1.readButton();
  display.print("But 1: ");
  display.print(button1.buttonState);
  display.println("");
  button2.readButton();
  display.print("But 2: ");
  display.print(button2.buttonState);
  display.println("");
  button3.readButton();
  display.print("But 3: ");
  display.print(button3.buttonState);
  display.println("");
  button4.readButton();
  display.print("But 4: ");
  display.print(button4.buttonState);
  display.println("");
  display.display();
}
