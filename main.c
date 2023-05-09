#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>()

#define MAX_EXPR_LEN 100
#define MAX_VAR_LEN 10

typedef struct {
    char* name;
    double value;
} Variable;

typedef struct {
    char op;
    int priority;
} Operator;

const Operator operators[] = {
        {'+', 1},
        {'-', 1},
        {'*', 2},
        {'/', 2},
        {'%', 2},
        {'^', 3}
};

const int num_operators = sizeof(operators) / sizeof(Operator);
Variable vars[MAX_VAR_LEN];
int num_vars = 0;

int is_operator(char c) {
    int i;
    for (i = 0; i < num_operators; i++) {
        if (operators[i].op == c) {
            return 1;
        }
    }
    return 0;
}

int get_operator_priority(char op) {
    int i;
    for (i = 0; i < num_operators; i++) {
        if (operators[i].op == op) {
            return operators[i].priority;
        }
    }
    return 0;
}

int is_variable(char* str) {
    int i;
    for (i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, str) == 0) {
            return 1;
        }
    }
    return 0;
}

double get_variable_value(char* str) {
    int i;
    for (i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, str) == 0) {
            return vars[i].value;
        }
    }
    return 0;
}

void set_variable_value(char* str, double value) {
    int i;
    for (i = 0; i < num_vars; i++) {
        if (strcmp(vars[i].name, str) == 0) {
            vars[i].value = value;
            return;
        }
    }
    vars[num_vars].name = strdup(str);
    vars[num_vars].value = value;
    num_vars++;
}

double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '%': return (int)a % (int)b;
        case '^': return pow(a, b);
    }
    return 0;
}

double evaluate_expression(char* expr) {
    double num_stack[MAX_EXPR_LEN];
    char op_stack[MAX_EXPR_LEN];
    int num_top = -1;
    int op_top = -1;
    int i;

    for (i = 0; expr[i] != '\0'; i++) {
        if (isdigit(expr[i])) {
            double num = expr[i] - '0';
            while (isdigit(expr[i + 1])) {
                num = num * 10 + (expr[i + 1] - '0');
                i++;
            }
            num_top++;
            num_stack[num_top] = num;
        } else if (isalpha(expr[i])) {
            char var[MAX_VAR_LEN];
            int j = 0;
            while (isalpha(expr[i]) || isdigit(expr[i])) {
                var[j] = expr[i];
                i++;
                j++;
            }
            var[j] = '\0';
            if (is_variable(var)) {
                double num = get_variable_value(var);
                num_top++;
                num_stack[num_top] = num;
            } else {
                printf("Undefined variable: %s\n", var);
                exit(1);
            }
            i--;
        } else if (expr[i] == '(') {
            op_top++;
            op_stack[op_top] = expr[i];
        } else if (expr[i] == ')') {
            while (op_top >= 0 && op_stack[op_top] != '(') {
                double b = num_stack[num_top];
                num_top--;
                double a = num_stack[num_top];
                num_top--;
                char op = op_stack[op_top];
                op_top--;
                double result = calculate(a, b, op);
                num_top++;
                num_stack[num_top] = result;
            }
            op_top--;
        } else if (is_operator(expr[i])) {
            char op = expr[i];
            while (op_top >= 0 && op_stack[op_top] != '(' &&
                   get_operator_priority(op_stack[op_top]) >= get_operator_priority(op)) {
                double b = num_stack[num_top];
                num_top--;
                double a = num_stack[num_top];
                num_top--;
                char op2 = op_stack[op_top];
                op_top--;
                double result = calculate(a, b, op2);
                num_top++;
                num_stack[num_top] = result;
            }
            op_top++;
            op_stack[op_top] = op;
        } else if (expr[i] == '=') {
            char var[MAX_VAR_LEN];
            int j = 0;
            while (isalpha(expr[i + 1]) || isdigit(expr[i + 1])) {
                var[j] = expr[i + 1];
                i++;
                j++;
            }
            var[j] = '\0';
            double value = evaluate_expression(expr + i + 2);
            set_variable_value(var, value);
            return value;
        }
    }

    while (op_top >= 0) {
        double b = num_stack[num_top];
        num_top--;
        double a = num_stack[num_top];
        num_top--;
        char op = op_stack[op_top];
        op_top--;
        double result = calculate(a, b, op);
        num_top++;
        num_stack[num_top] = result;
    }

    return num_stack[num_top];
}

int main() {
    char expr[MAX_EXPR_LEN];
    printf("Enter expression: ");
    fgets(expr, MAX_EXPR_LEN, stdin);
    double result = evaluate_expression(expr);
    printf("Result: %lf\n", result);
    return 0;
}
