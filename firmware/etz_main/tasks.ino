void task1() {
  find_line();

  draw_rect(3, 3, 267, 167);

  home();
}
void task2() {
  find_line();

  double radius = 170 / 3;
  while (radius > 3) {
    draw_circle(270 / 2, 170 / 2, radius);
    radius *= 0.8;
  }

  home();
}
void task3() {
  find_line();

  double start_x = -100, start_y = 5, dist_per_step = 5, x0 = -sqrt((170 - start_y) * 16) - start_x;
  set_pos(x0, (x0 + start_x) * (x0 + start_x) / 16 + start_y);
  //set_pos(x0 * dist_per_step, (x0 * dist_per_step - start_x) * (x0 * dist_per_step - start_x) + start_y);
  down_handle();
  while ((x0 + start_x) < 0 || (x0 + start_x) * (x0 + start_x) / 16 + start_y < 170) {
    set_pos_func(x0, (x0 + start_x) * (x0 + start_x) / 16 + start_y);
    x0 += dist_per_step;
    Serial.println(x0);
  }

  home();
}
void task4() {
  find_line();

  int x0 = 10, y0 = 52 - handle_displacement + 5 + 40, dir = 1;
  double pos[2 * 3];
  bool found = false, displacement = 0;
  set_pos(x0, y0);
  for (int i = 0; i < 3; i++) {
    while (y_curr < 175) {
      while (x_curr <= 270 && x_curr >= 7.5) {
        //set_pos(x0, y0);
        Vx = 50 * dir;
        wait();
        x0 += 5;
        if (analogRead(LINE_SENSOR) > line_value * 0.5) {
          stop_motor();
          light(0);
          delay(500);
          light(1);
          delay(500);
          found = true;
          displacement = true;
          break;
        }
      }
      stop_motor();
      if (found) break;
      move(-7.5 * dir, 5 + displacement * 7.5);
      displacement = 0;
      dir *= -1;
    }
    find_point(dir, pos[i * 2], pos[i * 2 + 1]);
    //draw_circle(pos[0], pos[1], 7.5, 1);
    /*pos[i * 2] = x_curr - 2.5 * dir + 6.7;
    pos[i * 2 + 1] = y_curr - 52 + handle_displacement + 2.75;*/
    move(12 * dir, 0);
    found = false;
  }
  /*move(-2.5 * dir, -52 + handle_displacement + 2.75);
  draw_circle(x_curr, y_curr, 6);*/
  //draw_point((pos[0] + pos[2]) / 2, (pos[1] + pos[3]) / 2);
  //draw_triangle(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);
  for (int i = 0; i < 3; i ++){
    draw_circle(pos[i * 2], pos[i * 2 + 1], 7.5, 1);
  }

  home();
}
void task5() {
  find_line();

  for (int i = 2; i < 20; i += 2) draw_function(100, 85, i);

  home();
}
void task6() {
  double x, y;
  find_point(1, x, y);
  draw_point(x, y);
}
void task7() {
  return;
}
void task8() {
  return;
}
void task9() {
  return;
}
void task10() {
  return;
}

void (*task_array[])() = { task1, task2, task3, task4, task5,
                           task6, task7, task8, task9, task10, show_parameters, calibr, home, test };