#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_EXPR_LEN 256

typedef enum { false, true } bool;

double eval(char *expr);

int main() {
    char expr[MAX_EXPR_LEN];
    printf("Введите выражение: ");
    fgets(expr, MAX_EXPR_LEN, stdin);
    double result = eval(expr);
    printf("Результат: %f\n", result);
    return 0;
}

double eval(char *expr) {
    char *end;
    double val;

    // пропустить начальные пробелы
    while (isspace(*expr)) {
        expr++;
    }

    // если выражение начинается со знака минус, добавить 0 перед выражением
    if (*expr == '-') {
        return 0 - eval(expr + 1);
    }

    // если выражение начинается со скобки, найти соответствующую закрывающую скобку
    if (*expr == '(') {
        int count = 1;
        char *p = expr + 1;
        while (*p != '\0') {
            if (*p == '(') {
                count++;
            } else if (*p == ')') {
                count--;
            }
            if (count == 0) {
                break;
            }
            p++;
        }
        if (*p == '\0') {
            printf("Ошибка: не найдена закрывающая скобка\n");
            exit(1);
        }
        *p = '\0';
        val = eval(expr + 1);
        *p = ')';
        expr = p + 1;
    } else {
        // если выражение начинается с числа или переменной, вычислить его значение
        val = strtod(expr, &end);
        if (end != expr) {
            expr = end;
        } else {
            // если выражение начинается с переменной, найти ее значение
            char *p = expr;
            while (isalnum(*p)) {
                p++;
            }
            if (p != expr) {
                char var[p - expr + 1];
                strncpy(var, expr, p - expr);
                var[p - expr] = '\0';
                printf("Введите значение переменной %s: ", var);
                scanf("%lf", &val);
                expr = p;
            } else {
                printf("Ошибка: ожидалось число или переменная\n");
                exit(1);
            }
        }
    }

    // выполнить операции с учетом приоритета
    while (true) {
        // пропустить пробелы
        while (isspace(*expr)) {
            expr++;
        }
        char op = *expr;
        if (op == '\0' || op == ')') {
            break;
        }
        if (strchr("+-*/^", op) == NULL) {
            printf("Ошибка: неизвестный оператор %c\n", op);
            exit(1);
        }
        expr++;

        // вычислить значение следующего операнда
        double next_val;
        if (op == '(') {
            next_val = eval(expr);
            while (*expr != ')') {
                expr++;
            }
            expr++;
        } else if (op == '-') {
            if (isdigit(*expr) || *expr == '.') {
                next_val = strtod(expr - 1, &end);
                expr = end;
            } else {
                next_val = eval(expr);
            }
            next_val = 0 - next_val;
        } else {
            // пропустить пробелы после оператора
            while (isspace(*expr)) {
                expr++;
            }
            // вычислить значение следующего операнда
            if (*expr == '(') {
                next_val = eval(expr);
                while (*expr != ')') {
                    expr++;
                }
                expr++;
            } else {
                next_val = strtod(expr, &end);
                if (end != expr) {
                    expr = end;
                } else {
                    char *p = expr;
                    while (isalnum(*p)) {
                        p++;
                    }
                    if (p != expr) {
                        char var[p - expr + 1];
                        strncpy(var, expr, p - expr);
                        var[p - expr] = '\0';
                        printf("Введите значение переменной %s: ", var);
                        scanf("%lf", &next_val);
                        expr = p;
                    } else {
                        printf("Ошибка: ожидалось число или переменная\n");
                        exit(1);
                    }
                }
            }
        }

        // выполнить операцию
        switch (op) {
            case '+':
                val += next_val;
                break;
            case '-':
                val -= next_val;
                break;
            case '*':
                val *= next_val;
                break;
            case '/':
                val /= next_val;
                break;
            case '^':
                val = pow(val, next_val);
                break;
        }
    }

    return val;
}