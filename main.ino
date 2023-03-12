#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TicTacToe.h"


#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

#define OLED_RESET -1
#define I2C_ADDR 0x3c

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TicTacToe TTT (&display);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDR); 
  TTT.play();
}

void loop() {
   
}
