void calibr() {
  uint32_t timer = millis();
  while (millis() - timer < 3000 && !button_pressed) delay(1);
}