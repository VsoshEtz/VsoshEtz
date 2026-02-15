int top = 130, bottom = 180, mid = 155;
bool handle_is_down = true;

void init_motors() {
  pinMode(MOTOR_DIR_X, OUTPUT);
  pinMode(MOTOR_STEP_X, OUTPUT);
  pinMode(MOTOR_DIR_Y, OUTPUT);
  pinMode(MOTOR_STEP_Y, OUTPUT);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ISR_X);
  Timer1.stop();

  Timer3.initialize(1000);
  Timer3.attachInterrupt(ISR_Y);
  Timer3.stop();
}
void init_servo() {
  handle.attach(SERVO_PIN);
  up_handle();
}
void init_serial() {
  Serial.begin(SERIAL_SPEED);
}

void ISR_X() {
  digitalWrite(MOTOR_STEP_X, step_state_x);
  digitalWrite(MOTOR_DIR_X, dir_x == 1 ? 1 : 0);
  x_curr += dir_x * step_state_x * DIST_PER_TICK_X / STEP_RESOLUTION;

  step_state_x = 1 - step_state_x;
}
void ISR_Y() {
  digitalWrite(MOTOR_STEP_Y, step_state_y);
  digitalWrite(MOTOR_DIR_Y, dir_y == 1 ? 1 : 0);
  y_curr += dir_y * step_state_y * DIST_PER_TICK_Y / STEP_RESOLUTION;

  step_state_y = 1 - step_state_y;
}

void motor(double speed_x, double speed_y) {
  static volatile double speed_x_last = 0, speed_y_last = 0;

  if (abs(speed_x) > DIST_PER_TICK_X / STEP_RESOLUTION) {
    if (speed_x > 0) dir_x = 1;
    else dir_x = -1;

    speed_x = abs(speed_x);
    speed_x = constrain(speed_x, 0, V);
    speed_x = 1000000 * DIST_PER_TICK_X / speed_x / 2 / STEP_RESOLUTION;

    if (speed_x_last != speed_x) {
      Timer1.setPeriod(speed_x);
      Timer1.start();
    }

    in_home = false;
  } else {
    Timer1.stop();
  }
  speed_x_last = speed_x;

  if (abs(speed_y) > DIST_PER_TICK_Y / STEP_RESOLUTION) {
    if (speed_y > 0) dir_y = 1;
    else dir_y = -1;

    speed_y = abs(speed_y);
    speed_y = constrain(speed_y, 0, V);
    speed_y = 1000000 * DIST_PER_TICK_Y / speed_y / 2 / STEP_RESOLUTION;

    if (speed_y_last != speed_y) {
      Timer3.setPeriod(speed_y);
      Timer3.start();
    }

    in_home = false;
  } else {
    Timer3.stop();
  }
  speed_y_last = speed_y;
}
void stop_motor() {
  motor(0, 0);
  delay(200);
  Vx = 0;
  Vy = 0;
}

double acceleration(double V_curr, double V_wish, double accel) {
  static double err;
  err = V_wish - V_curr;
  return min(accel * Ts, abs(err)) * sign(err);
}
int sign(double input) {
  if (input >= 0) return 1;
  else return -1;
}
void wait() {
  static uint32_t wait_timer;
  wait_timer = micros();
  while (micros() - wait_timer < Ts * 1000000) motor(Vx, Vy);
}
double dist_to_point(double x1, double y1, double x2 = x_curr, double y2 = y_curr) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void set_pos(double x, double y) {
  static double dS_x, dS_y, k, err_x, err_y, Vx_wish, Vy_wish;

  x = constrain(x, HOME_X, 280);
  y = constrain(y, 0, 180);

  dS_x = x - x_curr;
  dS_y = y - y_curr;
  //stop_motor();
  if (abs(dS_x) < 0.15) {
    k = 0;
    Vlim_y = V * sign(dS_y);
    Vlim_x = 0;
    ay = ACCEL;
    ax = 0;
  } else {
    k = abs(dS_y / dS_x);
    Vlim_x = V * sqrt(k * k + 1) / (k * k + 1) * sign(dS_x);
    Vlim_y = abs(Vlim_x) * k * sign(dS_y);
    ax = ACCEL * sqrt(k * k + 1) / (k * k + 1);
    ay = ax * k;
  }

  while (((x - x_curr) * sign(dS_x) > 0.35 || (y - y_curr) * sign(dS_y) > 0.35)) {
    err_x = (x - x_curr) * 15.5 + Vlim_x * 0.1;
    Vx_wish = min(abs(Vlim_x), abs(err_x)) * sign(dS_x);
    err_y = (y - y_curr) * 15.5 + Vlim_y * 0.1;
    Vy_wish = min(abs(Vlim_y), abs(err_y)) * sign(dS_y);

    Vx += acceleration(Vx, Vx_wish, ax);
    Vy += acceleration(Vy, Vy_wish, ay);
    wait();
  }
  stop_motor();
}
void set_pos_func(double x, double y) {
  static double dS_x, dS_y, k;

  x = constrain(x, HOME_X, 280);
  y = constrain(y, 0, 180);

  dS_x = x - x_curr;
  dS_y = y - y_curr;
  if (abs(dS_x) < 0.05) {
    k = 0;
    Vlim_y = V_FUNC * sign(dS_y);
    Vlim_x = 0;
  } else {
    k = abs(dS_y / dS_x);
    Vlim_x = V_FUNC * sqrt(k * k + 1) / (k * k + 1) * sign(dS_x);
    Vlim_y = abs(Vlim_x) * k * sign(dS_y);
  }
  Vx = Vlim_x;
  Vy = Vlim_y;

  while (((x - x_curr) * sign(dS_x) > 0.35 || (y - y_curr) * sign(dS_y) > 0.35)) {
    wait();
  }
}
void move(double dS_x, double dS_y) {
  set_pos(x_curr + dS_x, y_curr + dS_y);
}

void up_handle() {
  if (!handle_is_down) return;

  handle_is_down = false;
  handle.write(top);
  delay(250);
}
void down_handle() {
  if (handle_is_down) return;

  handle_is_down = true;
  handle.write(bottom);
  delay(250);
}
void set_handle() {
  delay(125);
  handle_is_down = true;
  while (!button_pressed) handle.write(mid);
  up_handle();
  button_pressed = false;
  hanlde_calibrated = true;
}
void init_led() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
}
void light(bool led_state) {
  digitalWrite(RED_LED, led_state);
}

void init_ktir() {
  pinMode(KTIR_PIN, INPUT_PULLUP);
}
void init_end_button() {
  pinMode(END_BUTTON, INPUT_PULLUP);
}

void calibr() {
  static const int sensor_reading = 100;

  // калибровка датчика света
  x_curr = 0;
  set_pos(0, FIND_LINE_X);
  line_value = 0;
  for (int times = 0; times < sensor_reading; times++) {
    line_value += analogRead(LINE_SENSOR);
    delay(10);
  }
  line_value /= sensor_reading;
  EEPROM.write(0, line_value / 4);

  //нахождение 0 по у
  while (!digitalRead(END_BUTTON)) {
    Vy = -40;
    wait();
  }
  y_curr = 0;

  //определение смещения маркера
  move(0, 20);
  handle.write(mid);
  delay(500);
  handle_is_down = true;
  while (!digitalRead(KTIR_PIN)) {
    Vy = -5;
    wait();
  }
  stop_motor();
  handle_displacement = y_curr;
  Serial.println(handle_displacement);
  EEPROM.write(1, (int) handle_displacement);

  //нахождение линии по у
  move(20, FIND_LINE_Y);
  while (analogRead(LINE_SENSOR) < line_value * 0.65) {
    Vy = -7.5;
    wait();
  }
  line_y_found = true;
  stop_motor();
  move(0, -52 + handle_displacement);
  up_handle();

  home_y = y_curr;
  EEPROM.write(2, (int)home_y);
  y_curr = 0;

  //установка домой
  set_pos(HOME_X, 0);
  in_home = true;
  current_state = 0;
  arrow_pos = 0;
}
void set_zero_y() {
  while (!digitalRead(END_BUTTON)) {
    Vy = -20;
    wait();
  }
  y_curr = 0;
  set_pos(HOME_X, home_y);
}