## Калькулятор
Данное приложение поможет вычислить результат выражения, переданное в виде строки, в которой есть **скобки**, **операции (+, -, \*, /)**,
**числа** с точностью до двух знаков после запятой.
### Компиляция
Чтобы скомпилировать программу, запустите компиляцию **make-файлом** (*который находится в корне проекта*), через терминал:
```bash
make
```
Чтобы удалить сгенерированные файлы, введите команду:
```bash
make clean
```
### Запуск программы
После того, как программа будет скомпилирована, её можно запустить. Чтобы запустить программу, введите команду, находясь в корне проекта:
```bash
./build/calculator
```
#### Работа программы
После запуска программы произойдёт тестирование программы. Результат тестирования будет выведен в консоль.
После тестирования программа будет ждать **строку** с арифметическим выражением.

#### Случай деления на 0
Программа в таком случае выведет **очень** большое число.