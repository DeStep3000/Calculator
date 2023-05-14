#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_EXPR_LEN 1000
#define MAX_VARS 100

typedef struct {
    char *name;
    double value;
} Variable;

Variable vars[MAX_VARS];

int num_vars = 0;

double get_variable_value(char *name) {
    int i;
    for (i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }
    return NAN;
}

void set_variable_value(char *name, double value) {
    int i;
    for (i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    vars[num_vars].name = strdup(name);
    vars[num_vars].value = value;
    num_vars++;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int get_operator_precedence(char c) {
    switch (c) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
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
        case '^':
            return pow(x, y);
        default:
            return NAN;
    }
}

double evaluate_expression(char *expr) {
    char operator_stack[MAX_EXPR_LEN];
    double value_stack[MAX_EXPR_LEN];
    int operator_top = -1;
    int value_top = -1;

    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        } else if (isdigit(expr[i])) {
            double value = 0;
            while (isdigit(expr[i])) {
                value = value * 10 + (expr[i] - '0');
                i++;
            }
            if (expr[i] == '.') {
                i++;
                double factor = 1;
                while (isdigit(expr[i])) {
                    factor *= 0.1;
                    value += (expr[i] - '0') * factor;
                    i++;
                }
            }
            value_stack[++value_top] = value;
        } else if (isalpha(expr[i])) {
            char name[MAX_EXPR_LEN];
            int j = 0;
            while (isalpha(expr[i]) || isdigit(expr[i]) || expr[i] == '_') {
                name[j++] = expr[i++];
            }
            name[j] = '\0';
            double value = get_variable_value(name);
            if (isnan(value)) {
                printf("Undefined variable: %s\n", name);
                return NAN;
            }
            value_stack[++value_top] = value;
        } else if (is_operator(expr[i])) {
            while (operator_top >= 0 && operator_stack[operator_top] != '(' &&
                   get_operator_precedence(operator_stack[operator_top]) >= get_operator_precedence(expr[i])) {
                double y = value_stack[value_top--];
                double x = value_stack[value_top--];
                char op = operator_stack[operator_top--];
                value_stack[++value_top] = apply_operator(op, x, y);
            }
            operator_stack[++operator_top] = expr[i];
            i++;
        } else if (expr[i] == '(') {
            operator_stack[++operator_top] = '(';
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
        if (operator_stack[operator_top] == '(' || operator_stack[operator_top] == ')') {
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

    return value_stack[0];
}

int main() {
    char expr[MAX_EXPR_LEN];
    printf("Enter an expression: ");
    fgets(expr, MAX_EXPR_LEN, stdin);
    printf("Result: %f\n", evaluate_expression(expr));
    return 0;
}
