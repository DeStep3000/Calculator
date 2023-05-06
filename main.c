#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_EXPR_LEN 100
#define EXPR_SIZE 100
#define VARIABLE_COUNT 10

// Структура для хранения переменных
typedef struct {
    char name;
    double value;
} Variable;

// Структура стека для операций
typedef struct {
    char op[MAX_EXPR_LEN];
    int top;
} Stack;

// Структура стека для чисел
typedef struct {
    double num[MAX_EXPR_LEN];
    int top;
} NumStack;

// Функция для добавления операции в стек операций
void push_op(Stack *stack, char op) {
    stack->op[++stack->top] = op;
}

// Функция для удаления операции из стека операций
char pop_op(Stack *stack) {
    return stack->op[stack->top--];
}

// Функция для проверки, является ли символ операцией
int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

// Функция для проверки, является ли символ открывающей скобкой
int is_left_paren(char c) {
    return c == '(';
}

// Функция для проверки, является ли символ закрывающей скобкой
int is_right_paren(char c) {
    return c == ')';
}

// Функция для получения приоритета операции
int precedence(char op) {
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

// Функция для добавления числа в стек чисел
void push_num(NumStack *stack, double num) {
    stack->num[++stack->top] = num;
}

// Функция для удаления числа из стека чисел
double pop_num(NumStack *stack) {
    return stack->num[stack->top--];
}

// Функция для проверки, является ли символ цифрой
int is_digit(char c) {
    return c >= '0' && c <= '9';
}

// Функция для проверки, является ли символ переменной
int is_variable(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Функция для получения выражения без пробелов
void remove_spaces(char *expr) {
    int i, j;
    for (i = 0, j = 0; expr[i]; i++) {
        if (expr[i] != ' ') {
            expr[j++] = expr[i];
        }
    }
    expr[j] = '\0';
}

// Функция для вычисления значения выражения
double evaluate(char *expr, Variable *variables, int var_count) {
    Stack op_stack;
    op_stack.top = -1;
    NumStack num_stack;
    num_stack.top = -1;
    int i;
    for (i = 0; expr[i]; i++) {
        if (is_digit(expr[i])) {
            // Если символ - цифра, добавляем число в стек чисел
            double num = 0.0;
            while (is_digit(expr[i])) {
                num = num * 10.0 + (expr[i] - '0');
                i++;
            }
            if (expr[i] == '.') {
                i++;
                double frac = 1.0;
                while (is_digit(expr[i])) {
                    frac /= 10.0;
                    num += frac * (expr[i] - '0');
                    i++;
                }
            }
            i--;
            push_num(&num_stack, num);
        } else if (is_variable(expr[i])) {
            // Если символ - переменная, добавляем значение переменной в стек чисел
            int j;
            for (j = 0; j < var_count; j++) {
                if (variables[j].name == expr[i]) {
                    push_num(&num_stack, variables[j].value);
                    break;
                }
            }
            if (j == var_count) {
                printf("Enter value for variable %c: ", expr[i]);
                double value;
                scanf("%lf", &value);
                variables[var_count].name = expr[i];
                variables[var_count].value = value;
                var_count++;
                push_num(&num_stack, value);
            }
        } else if (is_operator(expr[i])) {
            // Если символ - операция, добавляем ее в стек операций
            while (op_stack.top >= 0 && precedence(op_stack.op[op_stack.top]) >= precedence(expr[i])) {
                char op = pop_op(&op_stack);
                double num2 = pop_num(&num_stack);
                double num1 = pop_num(&num_stack);
                double result;
                switch (op) {
                    case '+':
                        result = num1 + num2;
                        break;
                    case '-':
                        result = num1 - num2;
                        break;
                    case '*':
                        result = num1 * num2;
                        break;
                    case '/':
                        result = num1 / num2;
                        break;
                    case '%':
                        result = fmod(num1, num2);
                        break;
                    case '^':
                        result = pow(num1, num2);
                        break;
                }
                push_num(&num_stack, result);
            }
            push_op(&op_stack, expr[i]);
        } else if (is_left_paren(expr[i])) {
            // Если символ - открывающая скобка, добавляем ее в стек операций
            push_op(&op_stack, expr[i]);
        } else if (is_right_paren(expr[i])) {
            // Если символ - закрывающая скобка, выполняем операции до открывающей скобки
            while (op_stack.top >= 0 && !is_left_paren(op_stack.op[op_stack.top])) {
                char op = pop_op(&op_stack);
                double num2 = pop_num(&num_stack);
                double num1 = pop_num(&num_stack);
                double result;
                switch (op) {
                    case '+':
                        result = num1 + num2;
                        break;
                    case '-':
                        result = num1 - num2;
                        break;
                    case '*':
                        result = num1 * num2;
                        break;
                    case '/':
                        result = num1 / num2;
                        break;
                    case '%':
                        result = fmod(num1, num2);
                        break;
                    case '^':
                        result = pow(num1, num2);
                        break;
                }
                push_num(&num_stack, result);
            }
            if (op_stack.top >= 0 && is_left_paren(op_stack.op[op_stack.top])) {
                pop_op(&op_stack);
            } else {
                printf("Error: mismatched parentheses\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Error: invalid character '%c'\n", expr[i]);
            exit(EXIT_FAILURE);
        }
    }

// Выполняем оставшиеся операции в стеке операций
    while (op_stack.top >= 0) {
        char op = pop_op(&op_stack);
        double num2 = pop_num(&num_stack);
        double num1 = pop_num(&num_stack);
        double result;
        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                result = num1 / num2;
                break;
            case '%':
                result = fmod(num1, num2);
                break;
            case '^':
                result = pow(num1, num2);
                break;
        }
        push_num(&num_stack, result);
    }

// Возвращаем значение выражения
    return pop_num(&num_stack);
}

int main() {
    char expr[EXPR_SIZE];
    printf("Enter an arithmetic expression: ");
    fgets(expr, EXPR_SIZE, stdin);
    // Убираем лишние пробелы из выражения
    remove_spaces(expr);

// Определяем переменные в выражении
    Variable variables[VARIABLE_COUNT];
    int var_count = 0;
    int i;
    for (i = 0; expr[i]; i++) {
        if (is_variable(expr[i])) {
            int j;
            for (j = 0; j < var_count; j++) {
                if (variables[j].name == expr[i]) {
                    break;
                }
            }
            if (j == var_count) {
                variables[var_count].name = expr[i];
                variables[var_count].value = 0.0;
                var_count++;
            }
        }
    }

// Вводим значения переменных
    for (i = 0; i < var_count; i++) {
        printf("Enter value for variable %c: ", variables[i].name);
        scanf("%lf", &variables[i].value);
    }

// Вычисляем значение выражения
    double result = evaluate(expr, variables, var_count);

// Выводим результат
    printf("Result: %g\n", result);

    return 0;
}

