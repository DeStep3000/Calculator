#include <stdio.h>
#include <stdlib.h>

// Структура для узла дерева
struct node {
    char op; // Оператор (+, -, *, /)
    int num; // Число
    struct node *left; // Левый потомок
    struct node *right; // Правый потомок
};

// Функция для создания нового узла
struct node* new_node(char op, int num) {
    struct node* node = (struct node*) malloc(sizeof(struct node));
    node->op = op;
    node->num = num;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Функция для вычисления значения выражения
int eval(struct node* node) {
    if (node->left == NULL && node->right == NULL) {
        return node->num;
    }
    int left_val = eval(node->left);
    int right_val = eval(node->right);
    switch (node->op) {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/': return left_val / right_val;
    }
    return 0;
}

// Функция для разбора выражения и создания дерева
struct node* parse_expr(char *expr, int len) {
    int i = 0;
    struct node* root = NULL;
    struct node* curr = NULL;
    int num = 0;
    char op = '+';
    while (i < len) {
        if (expr[i] >= '0' && expr[i] <= '9') {
            num = num * 10 + (expr[i] - '0');
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            if (curr == NULL) {
                root = new_node(op, num);
                curr = root;
            }
            else {
                curr->right = new_node(op, num);
                curr = curr->right;
            }
            num = 0;
            op = expr[i];
        }
        i++;
    }
    if (curr == NULL) {
        root = new_node(op, num);
    }
    else {
        curr->right = new_node(op, num);
    }
    return root;
}

int main() {
    char expr[] = "23-16+45*2";
    int len = sizeof(expr) / sizeof(expr[0]) - 1; // Вычисляем длину строки, не считая нулевой символ
    struct node* root = parse_expr(expr, len);
    int result = eval(root);
    printf("%d\n", result); // Выводим результат вычисления
    return 0;
}
