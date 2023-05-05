#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_EXPR_LENGTH 1024

double variables[26]; // массив для хранения значений переменных

double evaluate(char *expr);

int main() {
    char expr[MAX_EXPR_LENGTH];
    printf("Enter the expression: ");
    fgets(expr, MAX_EXPR_LENGTH, stdin);
    printf("Result: %f\n", evaluate(expr));
    return 0;
}

double calculate(double num1, double num2, char op) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
        case '^': return pow(num1, num2);
        default: return 0;
    }
}

double get_value(char *var) {
    if (*var < 'a' || *var > 'z') {
        return 0; // некорректное имя переменной
    }
    return variables[*var - 'a'];
}

double parse_number(char **expr) {
    double result = 0;
    while (isdigit(**expr)) {
        result = result * 10 + (**expr - '0');
        (*expr)++;
    }
    if (**expr == '.') {
        double factor = 0.1;
        (*expr)++;
        while (isdigit(**expr)) {
            result += factor * (**expr - '0');
            factor /= 10;
            (*expr)++;
        }
    }
    return result;
}

double parse_factor(char **expr) {
    double result;
    char var_name = **expr;
    if (isdigit(var_name)) {
        result = parse_number(expr);
    } else if (var_name >= 'a' && var_name <= 'z') {
        result = get_value(*expr);
        (*expr)++;
    } else if (var_name == '(') {
        (*expr)++;
        result = evaluate(*expr);
        if (**expr == ')') {
            (*expr)++;
        } else {
            printf("Error: unbalanced parentheses\n");
            exit(1);
        }
    } else {
        printf("Error: unexpected character '%c'\n", **expr);
        exit(1);
    }
    return result;
}

double parse_term(char **expr) {
    double result = parse_factor(expr);
    while (**expr == '*' || **expr == '/') {
        char op = **expr;
        (*expr)++;
        double num2 = parse_factor(expr);
        result = calculate(result, num2, op);
    }
    return result;
}

double evaluate(char *expr) {
    double result = parse_term(&expr);
    while (*expr) {
        char op = *expr;
        if (op == '+' || op == '-' || op == '^') {
            expr++;
            double num2 = parse_term(&expr);
            result = calculate(result, num2, op);
        } else {
            printf("Error: unexpected character '%c'\n", *expr);
            exit(1);
        }
    }
    return result;
}
