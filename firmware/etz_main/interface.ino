String lines[DISPLAY_LINES] = { "task1", "task2", "task3", "task4", "task5", "task6", "task7", "task8", "task9", "task10", "data", "calibr", "home", "test" };
extern void (*task_array[])();

int parameters_amount = 0, d_value = -1, a_value = -1, uart_value = -1;
String parameters_name[5];
int* parameters_value[5];
bool completed_tasks[DISPLAY_LINES];
bool task_completed = true;

int arrow_pos = 0;

struct Parameter {
  String name;
  int* value;
};
Parameter parameters[10];


void drawArrow(uint16_t y_arrow = 10) {
  uint16_t x_rect = 5, w_rect = 10, h_rect = 5;
  uint16_t x_triangle = x_rect + w_rect, w_triangle = 5, h_triangle = 10;
  display.fillRect(x_rect, y_arrow, w_rect, h_rect, SSD1306_WHITE);
  display.fillTriangle(x_triangle, y_arrow + h_rect / 2 - h_triangle / 2, x_triangle, y_arrow + h_rect / 2 + h_triangle / 2, x_triangle + w_triangle, y_arrow + h_rect / 2, SSD1306_WHITE);
}
void drawPlus(uint16_t y_plus) {
  uint16_t x_rect = 105, w_rect = 8, h_rect = 2;
  display.fillRect(x_rect, y_plus, w_rect, h_rect, SSD1306_WHITE);
  display.fillRect(x_rect + w_rect / 2 - h_rect / 2, y_plus + h_rect / 2 - w_rect / 2, h_rect, w_rect, SSD1306_WHITE);
}
void drawImage() {
  display.clearDisplay();
  for (uint16_t y_image = 0; y_image < IMG_HEIGHT; y_image++) {
    for (uint16_t x_image = 0; x_image < IMG_WIDTH; x_image++) {
      if (hello_kitty[x_image + y_image * IMG_WIDTH])
        display.drawPixel(x_image + SCREEN_WIDTH / 2 - IMG_WIDTH / 2, y_image + SCREEN_HEIGHT / 2 - IMG_HEIGHT / 2 + 8, SSD1306_WHITE);
    }
  }
  display.display();
}

void init_interface() {
  //энкодер
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN1), check_encoder, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), check_button, FALLING);
  pinMode(ENCODER_PIN2, INPUT);
  //дисплей
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  create_parameter("curr_state", current_state);
  create_parameter("arrow_pos", arrow_pos);
}
void create_parameter(String name, int& value) {
  parameters[parameters_amount].name = name;
  parameters[parameters_amount].value = &value;
  parameters_amount++;
  parameters_amount = constrain(parameters_amount, 0, 4);
}
void show_parameters() {
  while (!button_pressed) {
    display.clearDisplay();
    display.setTextSize(1);  // указываем размер шрифта
    for (int current_parameter = 0; current_parameter < parameters_amount; current_parameter++) {
      display.setTextSize(1);  // указываем размер шрифта
      display.setCursor(0, current_parameter * 12 + 4);
      display.print(parameters[current_parameter].name);
      display.setTextSize(2, 1);  // указываем размер шрифта
      display.setCursor(65, current_parameter * 12 + 4);
      display.print(*parameters[current_parameter].value);
    }
    display.display();
  }
  display.setTextSize(2);  // указываем размер шрифта
}
void set_task() {
  if (button_pressed) {
    button_pressed = false;
    task_completed = false;

    if (current_state != 10) drawImage();
    delay(500);
    for (int i = 0; i < 3; i++) {
      light(0);
      delay(333 / 2);
      light(1);
      delay(333 / 2);
    }
    task_array[current_state]();

    up_handle();
    for (int i = 0; i < 3; i++) {
      light(1);
      delay(333 / 2);
      light(0);
      delay(333 / 2);
    }

    button_pressed = false;
    task_completed = true;
  }
}

void check_encoder() {
  static uint32_t timer = millis();
  static int goida = 0;
  if (!task_completed) return;
  //if (millis() - timer < 75) return;
  /*goida = 0;
  for (int i = 0; i < 20; i++) {
    goida += digitalRead(ENCODER_PIN2);
    delayMicroseconds(200);
  }*/
  delay(1);
  if (digitalRead(ENCODER_PIN2) /*goida > 10*/) {
    current_state++;
    arrow_pos++;
  } else {
    current_state--;
    arrow_pos--;
  }
  current_state = constrain(current_state, 0, DISPLAY_LINES - 1);
  arrow_pos = constrain(arrow_pos, 0, 2);
  timer = millis();
  Serial.println(current_state);
  //current_state = (current_state + 12) % 12;
}
void check_button() {
  button_pressed = true;
  if (hanlde_calibrated) completed_tasks[current_state] = true;
}
void show_interface() {
  display.clearDisplay();  // очищаем экран
  for (int current_line = 0; current_line < DISPLAY_LINES; current_line++) {
    display.setCursor(30, (current_line - current_state + arrow_pos) * 21 + 5);
    display.print(lines[current_line]);
    if (completed_tasks[current_line] && current_line < 10)
      drawPlus((current_line - current_state + arrow_pos) * 21 + 11);
  }
  drawArrow(10 + arrow_pos * 21);
  display.display();
  delay(10);
}
