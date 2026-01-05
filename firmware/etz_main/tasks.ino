void task1(){
  Serial.println("goida");
  delay(1500);
}
void task2(){
  Serial.println("67");
  delay(1500);
}
void task3() {return;}
void task4() {return;}
void task5() {return;}
void task6() {return;}
void task7() {return;}
void task8() {return;}
void task9() {return;}
void task10() {return;}

void (*task_array[])() = {task1, task2, task3, task4, task5, 
                          task6, task7, task8, task9, task10, show_parameters, calibr, home};