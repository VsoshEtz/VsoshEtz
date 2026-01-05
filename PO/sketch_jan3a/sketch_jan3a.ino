#define DIST_PER_TICK_X 0.5024
#define DIST_PER_TICK_Y 0.5024
#define Ts 0.0001
#define ACCEL 4500.0
#define V 150

int step_y = 0, dir_y = 1;
double Vx = 0, Vy = 0, x_curr = 0, y_curr = 0;

double dist_to_point(double x1, double y1, double x2 = x_curr, double y2 = y_curr) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
double acceleration(int V_curr, int V_wish, int accel) {
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
  while (micros() - wait_timer < Ts)
    ;
}
void count_steps_y() {
  step_y += dir_y;
}
void motor(double speedX, double speedY) {
  if (speedX != 0) {
    digitalWrite(4, speedX > 0);
    speedX = abs(speedX);
    speedX = min(150, speedX);
    speedX = speedX / DIST_PER_TICK_X * 4;
    tone(5, speedX);
  } else {
    noTone(5);
  }

  if (speedY != 0) {
    speedY = DIST_PER_TICK_Y / speedY * 1000;
    digitalWrite(8, speedY > 0);
    speedY = abs(speedY);
    tone(7, min(300, speedY));
  } else {
    noTone(8);
  }
}
void setup() {
  attachInterrupt(digitalPinToInterrupt(18), count_steps_y, RISING);
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  delay(1000);
  Serial.println(dist_to_point(3, 4));
  Serial.println(dist_to_point(4, 5, 1, 1));
  Serial.println(dist_to_point(3, 4, 1, 6));
  /*for (int i = 0; i < 150; i += 1) {
    motor(i, 0);
    delayMicroseconds(500);
  }
  motor(300, 0);
  delay(1000);*/
  // put your setup code here, to run once:
}

void loop() {
  /*for (int i = 150; i > -150; i -= 5) {
    motor(i, 0);
    delayMicroseconds(100);
  }
  motor(-300, 0);
  delay(1000);
  for (int i = -150; i < 150; i += 5) {
    motor(i, 0);
    delayMicroseconds(100);
  }
  motor(300, 0);
  delay(1000);*/
  // put your main code here, to run repeatedly:
}
