#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>

// Функция, возвращающая значение выражения, заданного строкой expr,
// с учетом значений переменных, заданных в словаре vars
double complex eval(char *expr, double complex *vars) {
    int n = strlen(expr);
    char op;
    double complex val1, val2;
    int i, j, k, l;

    // Стек для хранения значений выражений в скобках
    double complex stack[100];
    int top = -1;

    // Проходим по строке и вычисляем выражение
    for (i = 0; i < n; i++) {
        // Если символ является буквой, считаем, что это имя переменной или функции
        if (expr[i] >= 'a' && expr[i] <= 'z') {
            char name[20] = "";
            j = i;
            while (j < n && ((expr[j] >= 'a' && expr[j] <= 'z') || (expr[j] >= '0' && expr[j] <= '9'))) {
                strncat(name, &expr[j], 1);
                j++;
            }
            i = j - 1;

            // Если это имя функции, вычисляем ее значение и помещаем в стек
            if (strcmp(name, "cos") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции \"cos\"\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции \"cos\"\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double complex result = cos(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "sin") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции \"sin\"\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции \"sin\"\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double complex result = sin(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "tg") == 0) {// Функция tg еще не реализована
                printf("Функция tg не реализована\n");
                exit(1);
            } else if (strcmp(name, "log") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'log'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ',') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена запятая в вызове функции 'log'\n");
                    exit(1);
                }
                char arg1[100] = "";
                strncpy(arg1, &expr[j], k - j);
                double complex arg1val = eval(arg1, vars);
                j = k + 1;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'log'\n");
                    exit(1);
                }
                char arg2[100] = "";
                strncpy(arg2, &expr[j], k - j);
                double complex arg2val = eval(arg2, vars);
                double complex result = clog(arg1val) / clog(arg2val);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "ln") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'ln'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'ln'\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double complex result = clog(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "sqrt") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'sqrt'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'sqrt'\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double complex result = csqrt(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "pow") == 0) {
                // Функция pow еще не реализована
                printf("Функция pow не реализована\n");
                exit(1);
            } else if (strcmp(name, "abs") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'abs'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'abs'\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double result = cabs(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "exp") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'exp'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'exp'\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double complex result = cexp(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "real") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'real'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'real'\n");
                    exit(1);
                }
                char arg[100] = "";
                strncpy(arg, &expr[j], k - j);
                double complex argval = eval(arg, vars);
                double result = creal(argval);
                stack[++top] = result;
                i = k;
            } else if (strcmp(name, "imag") == 0) {
                if (expr[i + 1] != '(') {
                    printf("Ошибка: ожидалась открывающая скобка после имени функции 'imag'\n");
                    exit(1);
                }
                j = i + 2;
                k = j;
                while (k < n && expr[k] != ')') {
                    k++;
                }
                if (k >= n) {
                    printf("Ошибка: не найдена закрывающая скобка в вызове функции 'imag'\n");
                    exit(1);
                    char arg[100] = "";
                    strncpy(arg, &expr[j], k - j);
                    double complex argval = eval(arg, vars);
                    double result = cimag(argval);
                    stack[++top] = result;
                    i = k;
                } else if (strcmp(name, "mag") == 0) {
                    if (expr[i + 1] != '(') {
                        printf("Ошибка: ожидалась открывающая скобка после имени функции 'mag'\n");
                        exit(1);
                    }
                    j = i + 2;
                    k = j;
                    while (k < n && expr[k] != ')') {
                        k++;
                    }
                    if (k >= n) {
                        printf("Ошибка: не найдена закрывающая скобка в вызове функции 'mag'\n");
                        exit(1);
                    }
                    char arg[100] = "";
                    strncpy(arg, &expr[j], k - j);
                    double complex argval = eval(arg, vars);
                    double result = cabs(argval);
                    stack[++top] = result;
                    i = k;
                } else if (strcmp(name, "phase") == 0) {
                    if (expr[i + 1] != '(') {
                        printf("Ошибка: ожидалась открывающая скобка после имени функции 'phase'\n");
                        exit(1);
                    }
                    j = i + 2;
                    k = j;
                    while (k < n && expr[k] != ')') {
                        k++;
                    }
                    if (k >= n) {
                        printf("Ошибка: не найдена закрывающая скобка в вызове функции 'phase'\n");
                        exit(1);
                    }
                    char arg[100] = "";
                    strncpy(arg, &expr[j], k - j);
                    double complex argval = eval(arg, vars);
                    double result = carg(argval);
                    stack[++top] = result;
                    i = k;
                } else {
                    printf("Ошибка: неизвестная функция '%s'\n", name);
                    exit(1);
                }
            } else {
// Ошибка: неизвестный символ
                printf("Ошибка: неизвестный символ '%c'\n", expr[i]);
                exit(1);
            }
        }
        // Проверяем, что в стеке осталось ровно одно значение - результат вычислений
        if (top != 0) {
            printf("Ошибка: некорректное выражение\n");
            exit(1);
        }

// Возвращаем результат вычислений
        return stack[0];
    }
}

int main() {
// Задаем начальные значения переменных
    struct var vars[] = {
            {"len", 191.76},
            {"v0",  299.799},
            {"S11", -0.01 - 0.92I},
            {"S22", 0.0001 + 0.997I},
            {NULL,  0.0}
    };

// Считываем выражение с клавиатуры
    char expr[1000];
    printf("Введите выражение: ");
    fgets(expr, sizeof(expr), stdin);

// Удаляем символ перевода строки из введенной строки, если он есть
    int n = strlen(expr);
    if (n > 0 && expr[n - 1] == '\n') {
        expr[n - 1] = '\0';
        n--;
    }

// Вычисляем значение выражения
    double complex result = eval(expr, vars);

// Выводим результат
    printf("Результат: %.5f%+.5fj\n", creal(result), cimag(result));

    return 0;
}