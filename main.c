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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

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

int is_valid_name_char(char c, int i) {
    return isalnum(c) || (i > 0 && c == '_');
}

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

double get_variable_value(char *name) {
    for (int i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }
    printf("Undefined variable: %s\n", name);
    return NAN;
}

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

double evaluate_expression(char *expr) {
    int operator_top = -1;
    char operator_stack[MAX_EXPR_LEN];
    double value_stack[MAX_EXPR_LEN];
    int value_top = -1;
    int i = 0;
    while (expr[i] != '\0' && expr[i] != '\n') {
        if (isspace(expr[i])) {
            i++;
        } else if (isdigit(expr[i])) {
            char *endptr;
            double value = strtod(&expr[i], &endptr);
            value_stack[++value_top] = value;
            i = endptr - expr;
        }
        else if (isalpha(expr[i])) {
            char name[MAX_VAR_NAME_LEN];
            int j = 0;
            while (is_valid_name_char(expr[i], j)) {
                name[j++] = expr[i++];
            }
            name[j] = '\0';
            double value = get_variable_value(name);
            if (isnan(value)) {
                return NAN;
            }
            value_stack[++value_top] = value;
        } else if (is_operator(expr[i])) {
            char op = expr[i];
            while (operator_top >= 0 && is_operator(operator_stack[operator_top]) &&
                   get_operator_precedence(op) <= get_operator_precedence(operator_stack[operator_top])) {
                double y = value_stack[value_top--];
                double x = value_stack[value_top--];
                char op2 = operator_stack[operator_top--];
                value_stack[++value_top] = apply_operator(op2, x, y);
            }
            operator_stack[++operator_top] = op;
            i++;
        } else if (expr[i] == '(') {
            operator_stack[++operator_top] = expr[i];
            i++;
        } else if (expr[i] == ')') {
            while (operator_top >= 0 && operator_stack[operator_top] != '(') {
                double y = value_stack[value_top--];
                double x = value_stack[value_top--];
                char op = operator_stack[operator_top--];
                value_stack[++value_top] = apply_operator(op, x, y);
            }
            if (operator_top < 0 || operator_stack[operator_top] != '(') {
                printf("Mismatched parentheses\n");
                return NAN;
            }
            operator_top--;
            i++;
        } else {
            printf("Invalid character: %c\n", expr[i]);
            return NAN;
        }
    }
    while (operator_top >= 0) {
        if (operator_stack[operator_top] == '(') {
            printf("Mismatched parentheses\n");
            return NAN;
        }
        double y = value_stack[value_top--];
        double x = value_stack[value_top--];
        char op = operator_stack[operator_top--];
        value_stack[++value_top] = apply_operator(op, x, y);
    }
    if (value_top != 0 || operator_top != -1) {
        printf("Invalid expression\n");
        return NAN;
    }
    return value_stack[value_top];
}

int main() {
    char expr[MAX_EXPR_LEN];
    printf("Enter an expression:\n");
    fgets(expr, MAX_EXPR_LEN, stdin);
    set_variable_value("x", 3.14);
    set_variable_value("y", 2.0);
    double result = evaluate_expression(expr);
    printf("Result: %g\n", result);
    return 0;
}