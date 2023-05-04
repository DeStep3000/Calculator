#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tstack.h"

int alphabet(char c) {
    switch (c) {
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case '0': return 0;
    }
    return -1;
}

int Cheching_for_a_Num(char a) {
    return (a >= '0' && a <= '9');
}

int get_operator_priority(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    else {
        return 0;
    }
}

int ifOperator(char c) {
    switch (c) {
        case '+': return 1;
        case '-': return 2;
        case '*': return 3;
        case '/': return 4;
    }
    return 0;
}

char* infx2pstfx(char* inf) {
    TStack s;
    s.top = NULL;
    char* result = (char*)malloc(sizeof(char) * strlen(inf) * 2);
    int j = 0;
    for (int i = 0; i < strlen(inf); i++) {
        char c = inf[i];
        if (Cheching_for_a_Num(c)) {
            result[j] = c;
            j++;
        }
        else if (c == '(') {
            push(&s, c);
        }
        else if (c == ')') {
            while (!isEmpty(s) && peek(s) != '(') {
                result[j] = peek(s);
                j++;
                pop(&s);
            }
            pop(&s);
        }
        else if (ifOperator(c)) {
            while (!isEmpty(s) && \
            get_operator_priority(peek(s)) >= get_operator_priority(c)) {
                result[j] = peek(s);
                j++;
                pop(&s);
            }
            push(&s, c);
        }
    }
    while (!isEmpty(s)) {
        result[j] = peek(s);
        j++;
        pop(&s);
    }
    result[j] = '\0';
    char* end = (char*)malloc(sizeof(char) * strlen(result) * 2);
    j = 0;
    for (int i = 0; i < strlen(result); i++) {
        if (i < strlen(result) - 1) {
            end[j] = result[i];
            j++;
            end[j] = ' ';
            j++;
        }
        else {
            end[j] = result[i];
            j++;
        }
    }
    end[j] = '\0';
    return end;
}

int eval(char* post) {
    TStack s;
    int i = 0;
    while (post[i]) {
        char c = post[i];
        if (Cheching_for_a_Num(c)) {
            push(&s, alphabet(c));
        }
        else {
            int temp2 = Get(&s);
            Pop(&s);
            int temp1 = Get(&s);
            Pop(&s);
            switch (c) {
                case '+':
                    push(&s, temp1 + temp2);
                    break;
                case '-':
                    push(&s, temp1 - temp2);
                    break;
                case '*':
                    push(&s, temp1 * temp2);
                    break;
                case '/':
                    push(&s, temp1 / temp2);
                    break;
            }
        }
        i++;
    }
    return Get(&s);
}

int main() {
    setlocale(0, "rus");
    char expr[] = "(5+2)/6-(4+3)*5";
    char postfix_expr[100];
    infx2pstfx(expr, postfix_expr);
    char a[] = "a a";
    char b[] = "aa";
    printf("Инфиксная форма: %s\n", expr);
    printf("Постфиксная форма: %s\n", postfix_expr);
    printf("Результат: %d\n", eval(postfix_expr));
    printf("%d\n", strcmp(a, b) == 0);
    return 0;
}