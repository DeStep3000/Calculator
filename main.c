#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

double eval(char *expr);

int main()
{
    char expr[256];
    printf("Введите выражение: ");
    fgets(expr, sizeof(expr), stdin);

    double result = eval(expr);
    printf("Результат: %g\n", result);

    return 0;
}

double eval(char *expr)
{
    double val, next_val;
    char op, *end;

    // вычислить первый операнд
    val = strtod(expr, &end);
    if (end != expr) {
        expr = end;
    } else {
        char var[256];
        int i = 0;
        while (isalnum(*expr)) {
            var[i++] = *expr++;
        }
        var[i] = '\0';
        printf("Введите значение переменной %s: ", var);
        scanf("%lf", &val);
    }

    while (*expr) {
        // пропустить пробелы перед оператором
        while (isspace(*expr)) {
            expr++;
        }
        // прочитать оператор
        op = *expr;
        if (op == '\0') {
            break;
        } else if (op == '+' || op == '-') {
            // обработать операции сложения и вычитания
            expr++;
            next_val = strtod(expr, &end);
            if (end != expr) {
                expr = end;
            } else {
                char var[256];
                int i = 0;
                while (isalnum(*expr)) {
                    var[i++] = *expr++;
                }
                var[i] = '\0';
                printf("Введите значение переменной %s: ", var);
                scanf("%lf", &next_val);
            }
            if (op == '+') {
                val += next_val;
            } else {
                val -= next_val;
            }
        } else {
            // обработать операции умножения, деления и возведения в степень
            int op_prec;
            if (op == '*' || op == '/') {
                op_prec = 2;
            } else if (op == '^') {
                op_prec = 3;
            } else {
                printf("Ошибка: неизвестный оператор %c\n", op);
                exit(1);
            }
            while (*expr && isspace(*expr)) {
                expr++;
            }
            if (*expr == '+' || *expr == '-') {
                next_val = eval(expr);
            } else {
                next_val = strtod(expr, &end);
                if (end != expr) {
                    expr = end;
                } else {
                    char var[256];
                    int i = 0;
                    while (isalnum(*expr)) {
                        var[i++] = *expr++;
                    }
                    var[i] = '\0';
                    printf("Введите значение переменной %s: ", var);
                    scanf("%lf", &next_val);
                }
            }
            while (*expr && isspace(*expr)) {
                expr++;
            }
            while (*expr && (*expr == op || *expr == '^')) {
                double tmp_val;
                char tmp_op = *expr;
                expr++;
                if (tmp_op == '+' || tmp_op == '-') {
                    tmp_val = eval(expr);
                } else {
                    tmp_val = strtod(expr, &end);
                    if (end != expr) {
                        expr = end;
                    } else {
                        char var[256];
                        int i = 0;
                        while (isalnum(*expr)) {
                            var[i++] = *expr++;
                        }
                        var[i] = '\0';
                        printf("Введите значение переменной %s: ", var);
                        scanf("%lf", &next_val);
                    }
                }
                while (*expr && isspace(*expr)) {
                    expr++;
                }
                if (op == '*') {
                    val *= next_val;
                } else if (op == '/') {
                    val /= next_val;
                } else if (op == '^') {
                    val = pow(val, next_val);
                } else {
                    printf("Ошибка: неизвестный оператор %c\n", op);
                    exit(1);
                }
            }
        }

        return val;
    }