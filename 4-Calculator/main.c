#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUF_SIZE (1000)

typedef struct _Stack {
    int array[BUF_SIZE];
    int size;
} Stack;

void push(Stack *stk, int elem) {
    stk -> array[stk -> size] = elem;
    stk -> size++;
}

int pop(Stack *stk) {
    if (stk -> size == 0) return EOF;
    stk -> size--;
    return stk -> array[stk -> size];
}

int head_elem(Stack *stk) {
    if (stk -> size == 0) return EOF;
    return stk -> array[stk -> size - 1];
}

void show_error(char *message) {
    printf("%s", message);
    exit(0);
}

bool is_operator (char a) {
    return a == '+' || a == '-' || a == '*' || a == '/';
}

bool is_digit (char a) {
    return a >= '0' && a <= '9';
}

bool correct_symbol (char a) {
    return is_operator(a) || is_digit(a) || a == '(' || a == ')';
}

int priority (char a) {
    return a == '(' || a == ')' ? 1 :
           a == '+' || a == '-' ? 2 :
           a == '*' || a == '/' ? 3 : 4;
}

int get_int(const char *num) {
    return atoi(num);
}

int eval(int a, int b, char operator) {
    
    switch (operator) {
        case '+': return b + a;
        case '-': return b - a;
        case '*': return b * a;
        case '/': {
            if (a == 0) return -1;
            else return b / a;
        }
        default: return -2;
    }
}

void count(Stack *digits, Stack *operators) {

    int a = pop(digits);
    int b = pop(digits);

    char op = (char) pop(operators);

    int result = eval(a, b, op);

    // Проверка ошибок
    if (result == -1) {
        show_error("division by zero");
    }
    else if (result == -2) {
        show_error("syntax error");
    }

    push(digits, result);
}

int calculate(char *input, int in_len) {

    Stack stk_digits;
    stk_digits.size = 0;

    Stack stk_operators;
    stk_operators.size = 0;

    char num[16];

    for (int i = 0; i < in_len; ++i) {

        if (is_digit(input[i])) {

            memset(num, 0, sizeof(num));
            int k = 0;

            while (is_digit(input[i]))
            {
                num[k] = input[i];
                k++;
                i++;

                if (i == in_len) break;
            }

            i--;
            push(&stk_digits, get_int(num));
        }
        else if (is_operator(input[i])) {

            if (i == 0 || is_operator(input[i-1]) || i == in_len-1) {
                show_error("syntax error");
            }

            char head = (char) head_elem(&stk_operators);

            while (priority(input[i]) <= priority(head) &&
                   stk_operators.size != 0) {
                
                count(&stk_digits, &stk_operators);
                head = (char) head_elem(&stk_operators);
            }

            push(&stk_operators, input[i]);
        }
        else if (input[i] == '(') {
            push(&stk_operators, input[i]);
        }
        else {
            if (i == 0 || is_operator(input[i-1]) || input[i-1] == '(') {
                show_error("syntax error");
            }

            char head = (char) head_elem(&stk_operators);

            while (head != '(') {
                count(&stk_digits, &stk_operators);
                head = (char) head_elem(&stk_operators);
            }

            pop(&stk_operators);
        }
    }

    while (stk_operators.size > 0) {
        count(&stk_digits, &stk_operators);
    }

    return head_elem(&stk_digits);
}

bool check_input(char *input, int *i) {

    for (*i = 0; *i < BUF_SIZE; ++(*i)) {
        int res = fscanf(stdin, "%c", &input[*i]);

        if (input[*i] == '\n') {
            if (*i == 0) return false;
            else break;
        }

        if (!correct_symbol(input[*i]) || res == EOF) {
            return false;
        }
    }

    return true;
}

int main(void) {

    char input[BUF_SIZE];
    int in_len;

    // Чтение строки
    if (!check_input(input, &in_len)) {
        show_error("syntax error");
    }

    // Подсчёт результата
    printf("%d", calculate(input, in_len));

    return EXIT_SUCCESS;
}
