#include <stdio.h>

int calculate(char *str) {
    int num1 = 0, num2 = 0, result = 0;
    char op = '+';
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            num2 = num2 * 10 + (*str - '0');
        } else if (*str == '+' || *str == '-' || *str == '*' || *str == '/') {
            if (op == '+') {
                result += num1;
                num1 = num2;
            } else if (op == '-') {
                result += num1;
                num1 = -num2;
            } else if (op == '*') {
                num1 *= num2;
            } else if (op == '/') {
                num1 /= num2;
            }
            op = *str;
            num2 = 0;
        }
        str++;
    }
    if (op == '+') {
        result += num1;
        num1 = num2;
    } else if (op == '-') {
        result += num1;
        num1 = -num2;
    } else if (op == '*') {
        num1 *= num2;
    } else if (op == '/') {
        num1 /= num2;
    }
    result += num1;
    return result;
}

int main() {
    char str[100];
    printf("Enter the expression: ");
    scanf("%s", str);
    int result = calculate(str);
    printf("Result: %d\n", result);
    return 0;
}