import cv2
import numpy as np

# Загрузка изображения
image = cv2.imread('C:\\Users\\Dan4i\\Desktop\\hello_kitty.png')

# Проверка, загрузилось ли изображение
if image is None:
    print("Ошибка: Не удалось загрузить изображение")
    exit()
else:
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    threshold_value = 67
    _, binary = cv2.threshold(gray_image, threshold_value, 255, cv2.THRESH_BINARY)
    summ = 0
    # Показываем результат
    cv2.imshow('Бинарное ЧБ', binary)
    print(image)
    print("{")
    for y in binary:
        for x in y:
            print(1 - x//255, end=", ")
            summ += 1
        print()
    print("}")
    # Показываем оба изображения
    cv2.waitKey(0)
    cv2.destroyAllWindows()

# Отображение изображения
print(summ)
# Ожидание нажатия клавиши и закрытие окна
cv2.waitKey(0)
cv2.destroyAllWindows()