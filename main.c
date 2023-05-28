#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_EXPR_LEN 1000
#define MAX_VAR_NAME_LEN 50
#define MAX_VARS 100

struct Variable {
    char name[MAX_VAR_NAME_LEN];
    double value;
};

struct Variable vars[MAX_VARS];
int num_vars = 0;

// Проверяет, является ли символ оператором (+, -, *, /, ^, %)
int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

// Возвращает приоритет оператора
int get_operator_precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return -1;
    }
}

// Проверяет, является ли символ допустимым символом имени переменной
int is_valid_name_char(char c, int i) {
    return isalnum(c) || (i > 0 && c == '_');
}

// Проверяет, является ли строка допустимым именем переменной
int is_valid_name(char *name) {
    if (!isalpha(name[0])) {
        return 0;
    }
    int len = strlen(name);
    for (int i = 1; i < len; i++) {
        if (!is_valid_name_char(name[i], i)) {
            return 0;
        }
    }
    return 1;
}

// Возвращает значение переменной по имени
double get_variable_value(char *name) {
    for (int i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }
    printf("Undefined variable: %s\n", name);
    return NAN;
}

// Устанавливает значение переменной по имени
void set_variable_value(char *name, double value) {
    for (int i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    if (num_vars >= MAX_VARS) {
        printf("Too many variables\n");
        return;
    }
    if (!is_valid_name(name)) {
        printf("Invalid variable name: %s\n", name);
        return;
    }
    struct Variable var;
    strcpy(var.name, name);
    var.value = value;
    vars[num_vars++] = var;
}

// Выполняет операцию между двумя операндами в соответствии с оператором
double apply_operator(char op, double x, double y) {
    switch (op) {
        case '+':
            return x + y;
        case '-':
            return x - y;
        case '*':
            return x * y;
        case '/':
            return x / y;
        case '%':
            return fmod(x, y);
        case '^':
            return pow(x, y);
        default:
            return NAN;
    }
}

// Вычисляет значение арифметического выражения
double evaluate_expression(char *expr) {
    int operator_top = -1; // Верхняя граница стека операторов
    char operator_stack[MAX_EXPR_LEN]; // Стек операторов
    double value_stack[MAX_EXPR_LEN]; // Стек значений
    int value_top = -1; // Верхняя граница стека значений
    int i = 0; // Текущая позиция в выражении
    int unary_minus = 0; // Флаг для обработки унарного минуса

    while (expr[i] != '\0' && expr[i] != '\n') {
        if (isspace(expr[i])) {
            i++; // Пропускаем пробелы
        } else if (isdigit(expr[i])) {
            char *endptr;
            double value = strtod(&expr[i], &endptr); // Преобразуем подстроку в число
            value_stack[++value_top] = unary_minus ? -value : value; // Добавляем значение в стек значений
            unary_minus = 0; // Сбрасываем флаг унарного минуса
            i = endptr - expr; // Перемещаем позицию в выражении после числа
        }
        else if (isalpha(expr[i])) {
            char name[MAX_VAR_NAME_LEN];
            int j = 0;
            while (is_valid_name_char(expr[i], j)) {
                name[j++] = expr[i++]; // Формируем имя переменной
            }
            name[j] = '\0';
            double value = get_variable_value(name); // Получаем значение переменной по имени
            if (isnan(value)) {
                return NAN; // Если значение не найдено, возвращаем NAN
            }
            value_stack[++value_top] = unary_minus ? -value : value; // Добавляем значение в стек значений
            unary_minus = 0; // Сбрасываем флаг унарного минуса
        } else if (is_operator(expr[i])) {
            char op = expr[i]; // Получаем оператор
            if (op == '-' && (i == 0 || is_operator(expr[i - 1]) || expr[i - 1] == '(')) {
                unary_minus = 1; // Устанавливаем флаг унарного минуса
            } else {
                while (operator_top >= 0 && is_operator(operator_stack[operator_top]) &&
                       get_operator_precedence(op) <= get_operator_precedence(operator_stack[operator_top])) {
                    double y = value_stack[value_top--];
                    double x = value_stack[value_top--];
                    char op2 = operator_stack[operator_top--];
                    value_stack[++value_top] = apply_operator(op2, x, y); // Применяем оператор к значениям и добавляем результат в стек значений
                }
                operator_stack[++operator_top] = op; // Добавляем оператор в стек операторов
            }
            i++; // Перемещаем позицию в выражении на следующий символ
        } else if (expr[i] == '(') {
            operator_stack[++operator_top] = expr[i]; // Добавляем открывающую скобку в стек операторов
            i++; // Перемещаем позицию в выражении на следующий символ
        } else if (expr[i] == ')') {
            while (operator_top >= 0 && operator_stack[operator_top] != '(') {
                double y = value_stack[value_top--];
                double x = value_stack[value_top--];
                char op = operator_stack[operator_top--];
                value_stack[++value_top] = apply_operator(op, x, y); // Применяем оператор к значениям и добавляем результат в стек значений
            }
            if (operator_top < 0 || operator_stack[operator_top] != '(') {
                printf("Mismatched parentheses\n");
                return NAN; // Если скобки не согласованы, возвращаем NAN
            }
            operator_top--; // Удаляем открывающую скобку из стека операторов
            i++; // Перемещаем позицию в выражении на следующий символ
        } else {
            printf("Invalid character: %c\n", expr[i]);
            return NAN; // Если символ недопустим, возвращаем NAN
        }
    }

    while (operator_top >= 0) {
        if (operator_stack[operator_top] == '(') {
            printf("Mismatched parentheses\n");
            return NAN; // Если скобки не согласованы, возвращаем NAN
        }
        double y = value_stack[value_top--];
        double x = value_stack[value_top--];
        char op = operator_stack[operator_top--];
        value_stack[++value_top] = apply_operator(op, x, y); // Применяем оператор к значениям и добавляем результат в стек значений
    }

    if (value_top != 0 || operator_top != -1) {
        printf("Invalid expression\n");
        return NAN; // Если стеки значений и операторов не пусты, или стек значений содержит больше одного элемента, возвращаем NAN
    }

    return value_stack[value_top]; // Возвращаем результат вычисления выражения
}


int main() {
    char expr[MAX_EXPR_LEN];
    printf("Enter an expression: ");
    fgets(expr, MAX_EXPR_LEN, stdin);
    float num;
    int i, j, len;
    char variable[MAX_VAR_NAME_LEN]; // максимальная длина имени переменной
    len = strlen(expr);
    for (i = 0; i < len; i++) {
        if (isalpha(expr[i])) { // если символ является буквой
            for (j = i; j < len; j++) {
                if (!isalnum(expr[j])) { // если символ не является буквой или цифрой
                    strncpy(variable, expr + i, j - i);
                    variable[j - i] = '\0';
                    printf("Input %s: ", variable);
                    scanf("%f", &num);
                    set_variable_value(variable, num);
                    i = j;
                    break;
                }
            }
        }
    }
    double result = evaluate_expression(expr);
    printf("Result: %g\n", result);
    return 0;
}
