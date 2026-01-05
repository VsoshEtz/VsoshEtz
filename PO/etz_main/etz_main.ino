#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define IMG_WIDTH 61
#define IMG_HEIGHT 50
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DISPLAY_LINES 13

#define ENCODER_PIN1 3
#define ENCODER_PIN2 9
#define BUTTON_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

int current_state = 0;
bool button_pressed = false;
extern int arrow_pos;

void setup() {
  init_interface();
  // put your setup code here, to run once:
}

void loop() {
  show_interface();
  set_task();
  //last_state = current_state_button;
  // put your main code here, to run repeatedly:
}
