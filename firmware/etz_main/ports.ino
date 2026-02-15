void init_pin_D(int mode) {
  if (mode == 1) pinMode(D_PIN, INPUT);
  else if (mode == 2) pinMode(D_PIN, INPUT_PULLUP);
  else pinMode(D_PIN, OUTPUT);
}
void init_pin_A() {
  pinMode(A_PIN, INPUT);
}
void init_pin_UART() {
  SERIAL_PIN.begin(UART_SPEED, SERIAL_8N1);
}

int get_value_D() {
  return digitalRead(D_PIN);
}
int get_value_A() {
  return analogRead(A_PIN);
}
int get_value_UART() {
  static int last = -1;

  if (SERIAL_PIN.available()) last = SERIAL_PIN.read();
  return last;
}

void write_pin_D(bool level){
  digitalWrite(D_PIN, level);
}
void write_pin_UART(int value){
  SERIAL_PIN.write(value);
}