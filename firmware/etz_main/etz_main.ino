#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include <Servo.h>
#include <EEPROM.h>

#define MOTOR_STEP_X 11
#define MOTOR_DIR_X 10
#define MOTOR_STEP_Y 5
#define MOTOR_DIR_Y 4
#define DIST_PER_TICK_X 0.5024
#define DIST_PER_TICK_Y 0.20096
#define Ts 0.01
#define ACCEL 1750  //2273
#define V 125.0
#define V_FUNC 50.0
#define STEP_RESOLUTION 4
#define SERIAL_SPEED 9600
#define LINE_HEIGHT 1

#define IMG_WIDTH 61
#define IMG_HEIGHT 50
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DISPLAY_LINES 14

#define ENCODER_PIN1 2
#define ENCODER_PIN2 6
#define BUTTON_PIN 3
#define SERVO_PIN 9

#define GREEN_LED 27
#define RED_LED 29
#define KTIR_PIN 25
#define END_BUTTON 23

#define D_PIN 24
#define A_PIN A5
#define SERIAL_PIN Serial3
#define UART_SPEED 9600
#define MODE 1

#define HOME_X -150
#define LINE_SENSOR A0
#define FIND_LINE_X 105
#define FIND_LINE_Y 75

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);
Servo handle;

int current_state = 0;
bool button_pressed = false;
extern int arrow_pos;

uint32_t line_value = EEPROM.read(0) * 4;
double handle_displacement = EEPROM.read(1);
double home_y = 10;

volatile int step_x = 0, step_y = 0, dir_x = 1, dir_y = 1;
volatile double Vx = 0, Vy = 0, Vlim_x, Vlim_y, x_curr = HOME_X, y_curr = 0, ax, ay;
volatile bool step_state_x = false, step_state_y = false, hanlde_calibrated = false;

bool in_home = true, line_y_found = false;

void setup() {
  init_interface();
  init_serial();
  init_servo();
  init_led();
  init_ktir();
  init_end_button();
  init_motors();
  init_pin_A();
  init_pin_D(MODE);
  init_pin_UART();

  set_handle();
  set_zero_y();
}

void loop() {
  show_interface();
  set_task();
}
