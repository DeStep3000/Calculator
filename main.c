#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define EXPR_SIZE 100
#define VARIABLE_COUNT 10

typedef struct {
    char name;
    double value;
} Variable;

typedef struct {
    int top;
    char op[EXPR_SIZE];
} OpStack;

typedef struct {
    int top;
    double num[EXPR_SIZE];
} NumStack;


char pop_op(OpStack *stack) {
    return stack->op[stack->top--];
}

void push_num(NumStack *stack, double num) {
    stack->num[++stack->top] = num;
}

double pop_num(NumStack *stack) {
    return stack->num[stack->top--];
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

int is_left_paren(char c) {
    return c == '(';
}

int is_right_paren(char c) {
    return c == ')';
}

int is_variable(char c) {
    return isalpha(c);
}

void remove_spaces(char *str) {
    int i, j;
    for (i = 0, j = 0; str[i]; i++) {
        if (!isspace(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void push_op(OpStack *stack, char op) {
    NumStack num_stack = { .top = -1 };
    while (stack->top >= 0 && !is_left_paren(stack->op[stack->top]) &&
           ((op != '^' && is_operator(stack->op[stack->top])) ||
            (op == '^' && stack->op[stack->top] == '^') ||
            (op == '*' && stack->op[stack->top] == '/') ||
            (op == '/' && stack->op[stack->top] == '*') ||
            (op == '%' && stack->op[stack->top] == '/') ||
            (op == '+' && stack->op[stack->top] != '-') ||
            (op == '-' && stack->op[stack->top] != '+'))) {
        char popped_op = pop_op(stack);
        double num2 = pop_num(&num_stack);
        double num1 = pop_num(&num_stack);
        switch (popped_op) {
            case '+':
                push_num(&num_stack, num1 + num2);
                break;
            case '-':
                push_num(&num_stack, num1 - num2);
                break;
            case '*':
                push_num(&num_stack, num1 * num2);
                break;
            case '/':
                push_num(&num_stack, num1 / num2);
                break;
            case '%':
                push_num(&num_stack, fmod(num1, num2));
                break;
            case '^':
                push_num(&num_stack, pow(num1, num2));
                break;
        }
    }
    stack->op[++stack->top] = op;
}


double evaluate(char *expr, Variable *variables, int var_count) {
    OpStack op_stack = { .top = -1 };
    NumStack num_stack = { .top = -1 };
    int i, j;
    for (i = 0, j = 0; expr[i]; i++) {
        if (isdigit(expr[i]) || (expr[i] == '.')) {
            double num = strtod(&expr[i], NULL);
            push_num(&num_stack, num);
            while (isdigit(expr[i]) || (expr[i] == '.')) {
                i++;
            }
            i--;
        } else if (is_variable(expr[i])) {
            int k;
            for (k = 0; k < var_count; k++) {
                if (variables[k].name == expr[i]) {
                    break;
                }
            }
            if (k == var_count) {
                printf("Error: undefined variable '%c'\n", expr[i]);
                exit(EXIT_FAILURE);
            }
            push_num(&num_stack, variables[k].value);
        } else if (is_operator(expr[i])) {
            push_op(&op_stack, expr[i]);
        } else if (is_left_paren(expr[i])) {
            push_op(&op_stack, expr[i]);
        } else if (is_right_paren(expr[i])) {
            while (op_stack.top >= 0 && !is_left_paren(op_stack.op[op_stack.top])) {
                char op = pop_op(&op_stack);
                double num2 = pop_num(&num_stack);
                double num1 = pop_num(&num_stack);
                switch (op) {
                    case '+':
                        push_num(&num_stack, num1 + num2);
                        break;
                    case '-':
                        push_num(&num_stack, num1 - num2);
                        break;
                    case '*':
                        push_num(&num_stack, num1 * num2);
                        break;
                    case '/':
                        push_num(&num_stack, num1 / num2);
                        break;
                    case '%':
                        push_num(&num_stack, fmod(num1, num2));
                        break;
                    case '^':
                        push_num(&num_stack, pow(num1, num2));
                        break;
                }
            }
            if (op_stack.top < 0) {
                printf("Error: mismatched parentheses\n");
                exit(EXIT_FAILURE);
            }
            pop_op(&op_stack);
        } else {
            printf("Error: invalid character '%c'\n", expr[i]);
            exit(EXIT_FAILURE);
        }
    }
    while (op_stack.top >= 0) {
        if (is_left_paren(op_stack.op[op_stack.top])) {
            printf("Error: mismatched parentheses\n");
            exit(EXIT_FAILURE);
        }
        char op = pop_op(&op_stack);
        double num2 = pop_num(&num_stack);
        double num1 = pop_num(&num_stack);
        switch (op) {
            case '+':
                push_num(&num_stack, num1 + num2);
                break;
            case '-':
                push_num(&num_stack, num1 - num2);
                break;
            case '*':
                push_num(&num_stack, num1 * num2);
                break;
            case '/':
                push_num(&num_stack, num1 / num2);
                break;
            case '%':
                push_num(&num_stack, fmod(num1, num2));
                break;
            case '^':
                push_num(&num_stack, pow(num1, num2));
                break;
        }
    }
    if (num_stack.top != 0) {
        printf("Error: invalid expression\n");
        exit(EXIT_FAILURE);
    }
    return num_stack.num[num_stack.top];
}

int main() {
    char expr[EXPR_SIZE];
    Variable variables[VARIABLE_COUNT];
    int var_count = 0;
    printf("Enter expression: ");
    fgets(expr, EXPR_SIZE, stdin);
    remove_spaces(expr);
    int i;
    for (i = 0; expr[i]; i++) {
        if (is_variable(expr[i])) {
            int j;
            for (j = 0; j < var_count; j++) {
                if (expr[i] == variables[j].name) {
                    break;
                }
            }
            if (j == var_count) {
                variables[var_count].name = expr[i];
                printf("Enter value for variable %c: ", variables[var_count].name);
                scanf("%lf", &variables[var_count].value);
                var_count++;
            }
        }
    }
    double result = evaluate(expr, variables, var_count);
    printf("Result: %g\n", result);
    return 0;
}