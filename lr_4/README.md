# Поиск образца в строке

Поиск большого количества образцов при помощи алгоритма Ахо-Корасик.

Вариант алфавита: числа в диапазоне от 0 до 232 - 1.

## Формат входных данных
Искомый образец задается на первой строке входного файла.
В случае, если в задании требуется найти несколько образцов, они задаются по одному на строку вплоть до пустой строки.
Затем следует текст, состоящий из слов или чисел, в котором нужно найти заданные образцы.
Никаких ограничений на длину строк, равно как на количество слов или числ в них, не накладывается.

## Формат выходных данных
В выходной файл нужно вывести информацию о всех вхождениях искомых образцов в обрабатываемый текст: по одному вхождению на строчку.

Для заданий, в которых требуется найти только один образец, следует вывести два числа через запятую: номер строки и номер слова в строке, с которого начинается найденный образец. В заданиях с большим количеством образцов, на каждое вхождение нужно вывести три числа через запятую: номер строки; номер слова в строке, с которого начинается найденный образец; порядковый номер образца.

Нумерация начинается с единицы. Номер строки в тексте должен отсчитываться от его реального начала (то есть, без учёта строк, занятых образцами).

Порядок следования вхождений образцов несущественен.
