/*void check_x() {
  static bool last_state = 0;
  uint32_t timer = millis();
  while (millis() - timer < 2000) {
    bool current_state = digitalRead(24);
    if (current_state == 0 && last_state == 1) {
      delay(5);
      timer = millis();
      x++;
    }
    last_state = current_state;
  }
  x %= 10;
}*/
/*button_pressed = 0;
  while (!button_pressed){
    Serial.print(get_value_D());
    Serial.print("\t");
    Serial.print(get_value_A());
    Serial.print("\t");
    Serial.println(get_value_UART());
  }*/