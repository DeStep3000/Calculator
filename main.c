#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_EXPRESSION_LENGTH 100

typedef enum {
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_POWER,
} operator_type_t;

typedef struct {
    double value;
    char name[50];
} variable_t;

int precedence(operator_type_t op) {
    switch (op) {
        case OPERATOR_PLUS:
        case OPERATOR_MINUS:
            return 1;
        case OPERATOR_MULTIPLY:
        case OPERATOR_DIVIDE:
            return 2;
        case OPERATOR_POWER:
            return 3;
        default:
            return -1;
    }
}

double apply_operator(double left_operand, double right_operand, operator_type_t op) {
    switch (op) {
        case OPERATOR_PLUS:
            return left_operand + right_operand;
        case OPERATOR_MINUS:
            return left_operand - right_operand;
        case OPERATOR_MULTIPLY:
            return left_operand * right_operand;
        case OPERATOR_DIVIDE:
            return left_operand / right_operand;
        case OPERATOR_POWER:
            return pow(left_operand, right_operand);
        default:
            return 0.0;
    }
}

int is_operator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

int is_valid_name_char(char ch) {
    return isalnum(ch) || ch == '_';
}

variable_t* find_variable(variable_t* variables, int num_variables, char* name) {
    for (int i = 0; i < num_variables; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

double evaluate_expression(char* expression, variable_t* variables, int num_variables) {
    double operand_stack[MAX_EXPRESSION_LENGTH];
    operator_type_t operator_stack[MAX_EXPRESSION_LENGTH];
    int operand_stack_top = -1;
    int operator_stack_top = -1;
    int i = 0;
    while (expression[i] != '\0') {
        if (isdigit(expression[i])) {
            char* endptr;
            double value = strtod(&expression[i], &endptr);
            operand_stack[++operand_stack_top] = value;
            i = (int)(endptr - expression);
        } else if (isalpha(expression[i])) {
            char name[50];
            int name_length = 0;
            while (is_valid_name_char(expression[i])) {
                name[name_length++] = expression[i++];
            }
            name[name_length] = '\0';
            variable_t* variable = find_variable(variables, num_variables, name);
            if (variable == NULL) {
                printf("Error: undefined variable '%s'\n", name);
                return NAN;
            }
            operand_stack[++operand_stack_top] = variable->value;
        } else if (is_operator(expression[i])) {
            operator_type_t op;
            switch (expression[i]) {
                case '+':
                    op = OPERATOR_PLUS;
                    break;
                case '-':
                    op = OPERATOR_MINUS;
                    break;
                case '*':
                    op = OPERATOR_MULTIPLY;
                    break;
                case '/':
                    op = OPERATOR_DIVIDE;
                    break;
                case '^':
                    op = OPERATOR_POWER;
                    break;
                default:
                    printf("Error: invalid operator '%c'\n", expression[i]);
                    return NAN;
            }
            while (operator_stack_top >= 0 && precedence(operator_stack[operator_stack_top]) >= precedence(op)) {
                double right_operand = operand_stack[operand_stack_top--];
                double left_operand = operand_stack[operand_stack_top--];
                operator_type_t current_op = operator_stack[operator_stack_top--];
                operand_stack[++operand_stack_top] = apply_operator(left_operand, right_operand, current_op);
            }
            operator_stack[++operator_stack_top] = op;
            i++;
        } else if (expression[i] == '(') {
            operator_stack[++operator_stack_top] = OPERATOR_PLUS;
            i++;
        } else if (expression[i] == ')') {
            while (operator_stack_top >= 0 && operator_stack[operator_stack_top] != OPERATOR_PLUS) {
                double right_operand = operand_stack[operand_stack_top--];
                double left_operand = operand_stack[operand_stack_top--];
                operator_type_t current_op = operator_stack[operator_stack_top--];
                operand_stack[++operand_stack_top] = apply_operator(left_operand, right_operand, current_op);
            }
            if (operator_stack_top < 0) {
                printf("Error: unmatched parentheses\n");
                return NAN;
            }
            operator_stack_top--;
            i++;
        } else if (isspace(expression[i])) {
            i++;
        } else {
            printf("Error: invalid character '%c'\n", expression[i]);
            return NAN;
        }
    }
    while (operator_stack_top >= 0) {
        double right_operand = operand_stack[operand_stack_top--];
        double left_operand = operand_stack[operand_stack_top--];
        operator_type_t current_op = operator_stack[operator_stack_top--];
        operand_stack[++operand_stack_top] = apply_operator(left_operand, right_operand, current_op);
    }
    if (operand_stack_top != 0) {
        printf("Error: invalid expression\n");
        return NAN;
    }
    return operand_stack[0];
}

int main() {
    char expression[MAX_EXPRESSION_LENGTH];
    variable_t variables[10];
    int num_variables = 0;
    printf("Enter an expression: ");
    fgets(expression, MAX_EXPRESSION_LENGTH, stdin);
    if (expression[strlen(expression)-1] == '\n') {
        expression[strlen(expression)-1] = '\0';
    }
    while (1) {
        char name[50];
        printf("Enter a variable name (or 'done' to finish): ");
        fgets(name, 50, stdin);
        if (name[strlen(name)-1] == '\n') {
            name[strlen(name)-1] = '\0';
        }
        if (strcmp(name, "done") == 0) {
            break;
        }
        variable_t variable;
        strcpy(variable.name, name);
        printf("Enter the value of '%s': ", variable.name);
        scanf("%lf", &variable.value);
        getchar(); // consume the newline character
        variables[num_variables++] = variable;
    }
    double result = evaluate_expression(expression, variables, num_variables);
    printf("Result: %lf\n", result);
    return 0;
}