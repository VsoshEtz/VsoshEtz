void home() {
  if (in_home) return;

  handle_is_down = true;
  up_handle();
  set_pos(HOME_X, 0);
  in_home = true;
}

void find_line() {
  //по х
  set_pos(HOME_X, FIND_LINE_X);
  while (analogRead(LINE_SENSOR) < line_value) {
    Vx = 75;
    wait();
  }
  motor(60, 0);
  delay(300);
  while (analogRead(LINE_SENSOR) < line_value) {
    Vx = -10;
    wait();
  }
  x_curr = 0;
  stop_motor();

  //по у
  if (!line_y_found) {
    set_pos(15, FIND_LINE_Y);
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
    line_y_found = true;
  }
}
void draw_point(double start_x, double start_y) {
  up_handle();
  set_pos(start_x, start_y);
  down_handle();
  delay(200);
  up_handle();
}
void draw_line(double start_x, double start_y, double end_x, double end_y) {
  if (dist_to_point(start_x, start_y) > 1) {
    up_handle();
    set_pos(start_x, start_y);
  }
  down_handle();
  set_pos(end_x, end_y);
  up_handle();
}
void draw_many_lines(double end_x, double end_y, double start_x = -1, double start_y = -1) {
  static double last_x = 0, last_y = 0;

  if (start_x == -1) start_x = last_x;
  if (start_y == -1) start_y = last_y;

  if (dist_to_point(start_x, start_y) > 1) {
    up_handle();
    set_pos(start_x, start_y);
  }
  down_handle();
  set_pos(end_x, end_y);

  last_x = end_x;
  last_y = end_y;
}
void draw_dS_line(double start_x, double start_y, double dS_x, double dS_y) {
  if (dist_to_point(start_x, start_y) > 1) {
    up_handle();
    set_pos(start_x + sign(dS_x) * LINE_HEIGHT, start_y + sign(dS_y) * LINE_HEIGHT);
  }
  down_handle();
  move(dS_x - sign(dS_x) * LINE_HEIGHT, dS_y - sign(dS_y) * LINE_HEIGHT);
  up_handle();
}
void draw_length_line(double start_x, double start_y, double length, double angle) {
  angle = angle / 180 * PI;
  set_pos(start_x + sign(cos(angle)) * LINE_HEIGHT, start_y + sign(sin(angle)) * LINE_HEIGHT);
  down_handle();
  move(length * cos(angle) - sign(cos(angle)) * LINE_HEIGHT, length * sin(angle) - sign(sin(angle)) * LINE_HEIGHT);
  up_handle();
}
void draw_rect(double start_x, double start_y, double width, double height, double angle = 0) {
  angle = angle * PI / 180;
  draw_many_lines(start_x + width * cos(angle), start_y + width * sin(angle), start_x, start_y);
  draw_many_lines(start_x + width * cos(angle) + height * sin(angle), start_y + width * sin(angle) + height * cos(angle));
  draw_many_lines(start_x + height * sin(angle), start_y + height * cos(angle));
  draw_many_lines(start_x, start_y);
  up_handle();
}
void draw_triangle(double apex1_x, double apex1_y, double apex2_x, double apex2_y, double apex3_x, double apex3_y) {
  draw_many_lines(apex2_x, apex2_y, apex1_x, apex1_y);
  draw_many_lines(apex3_x, apex3_y);
  draw_many_lines(apex1_x, apex1_y);
  up_handle();
}
void draw_circle(double x_center, double y_center, double R, bool point=0, double k = 1) {
  static double x_new, y_new;
  static double a = R / sqrt(k), b = R, length = PI * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));  // Приближённая формула Рамануджана
  static int n = length / 2.5;

  if (point) draw_point(x_center, y_center);
  x_new = x_center + cos(PI) * R * k;
  y_new = y_center + sin(PI) * R;
  set_pos(x_new, y_new);
  down_handle();

  for (int i = 1; i <= n * 2; i++) {
    x_new = x_center + cos(PI / n * i + PI) * R * k;
    y_new = y_center + sin(PI / n * i + PI) * R;
    set_pos_func(x_new, y_new);
  }
  stop_motor();
  up_handle();
}
void draw_hui(double x, double y, double eggs, double length, double width) {
  draw_circle(x, y, eggs);
  draw_circle(x + eggs * 2, y, eggs);
  draw_circle(x + eggs, y - length, length, width / length);
}
void draw_function(double center_x, double center_y, double scale = 1) {
  static double const_part, changing_part;
  static double radius = 5;
  static bool start;
  start = false;
  changing_part = -1;

  for (double x = -sqrt(radius); x < sqrt(radius); x += 0.05) {
    const_part = pow(x * x, 1.0 / 3);
    changing_part = sqrt(radius - x * x);
    if (!start) {
      set_pos(center_x + x * scale, center_y - (const_part + changing_part) * scale);
      down_handle();
      start = true;
    } else {
      set_pos_func(center_x + x * scale, center_y - (const_part + changing_part) * scale);
    }
  }

  if (changing_part == -1) return;

  for (double x = sqrt(radius); x > -sqrt(radius); x -= 0.05) {
    const_part = cbrt(x * x);
    changing_part = -sqrt(radius - x * x);
    set_pos_func(center_x + x * scale, center_y - (const_part + changing_part) * scale);
  }
  up_handle();
}
void find_point(int dir, double& center_x, double& center_y){
  static double top, bottom, left, right;
  //нахождение середины по х
  //set_pos(x_curr, (bottom + top) / 2);
  move(3 * dir, 0);
  while (analogRead(LINE_SENSOR) > line_value * 0.3){
    Vx = 10 * dir;
    wait();
  }
  left = x_curr;
  stop_motor();
  move(-3 * dir, 0);
  while (analogRead(LINE_SENSOR) > line_value * 0.3){
    Vx = -10 * dir;
    wait();
  }
  stop_motor();
  right = x_curr;
  center_x = (left + right) / 2 - 1.5/* * (dir == 1)*/;

  //нахождение середины по у
  set_pos((right + left) / 2, y_curr);
  move(0, 10);
  while (analogRead(LINE_SENSOR) < line_value * 0.3){
    Vy = -5;
    wait();
  }
  move(0, -3);
  while (analogRead(LINE_SENSOR) > line_value * 0.3){
    Vy = -5;
    wait();
  }
  top = y_curr;
  stop_motor();
  move(0, 5.5);
  while (analogRead(LINE_SENSOR) > line_value * 0.3){
    Vy = 5;
    wait();
  }
  bottom = y_curr;
  center_y = (bottom + top) / 2 - 52 + handle_displacement - 0.75;

  set_pos((right + left) / 2, (bottom + top) / 2);
}

void test(){
  find_line();

  draw_many_lines(5, 5, 5, 155);
  draw_many_lines(155, 5);

  home();
}