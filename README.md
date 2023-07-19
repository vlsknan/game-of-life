# The game of life

## Групповой проект
Цель проекта - на языке программирования C разработать визуализацию игры "The game of life". Для отображения графики использовалась символьная графика (ASCII) с выводом в терминал. 

Игра реализована в интерактивном режиме с использованием библиотеки <ncurses.h>. Также с помощью клавиш "1" и "2" происходит настройка скорости игры. Начальное состояние определяется в файлах config.txt и подключается через направление ввода.

#### Правила игры "The game of life":
1. Выживает любая живая клетка с двумя-тремя живыми соседями.

2. Любая мертвая клетка с тремя живыми соседями становится живой клеткой.

3. Все остальные живые клетки погибают в следующем поколении. Точно так же все другие мертвые клетки остаются мертвыми.