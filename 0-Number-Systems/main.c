#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

const char digits[17] = "0123456789abcdef";

char lower(char symbol) {
    return (char)tolower(symbol);
}

int num_index(const char string[], char numeral) {
    return (int)(strchr(string, lower(numeral)) - string);
}

bool check_input(int variable_count, int b1, int b2, char* number_old) {

    int b1_len = (int)strlen(number_old);

    bool incorrect_num = false;
    int dots_count = 0;

    for (int i = 0; i < b1_len; ++i) {

        char numeral = lower(number_old[i]);

        if (numeral == '.') {
            dots_count++;
        }

        else if (!strchr(digits, numeral) || strchr(digits, numeral) - digits >= b1) {
            incorrect_num = true;
        }
    }

    if (variable_count != 3 || number_old[0] == '.' || number_old[b1_len - 1] == '.' ||
        b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16 || incorrect_num || dots_count > 1) {
        return false;
    }
    else {
        return true;
    }
}

bool check_frac_part(char *number_old) {

    int b1_len = (int)strlen(number_old);

    for (int i = 0; i < b1_len; ++i) {
        if (number_old[i] == '.')
            return true;
    }

    strcat(number_old, ".0");
    return true;
}

void transfer_to_decimal_system(char *number_old, int b1,
        double *int_part, double *frac_part) {

    int b1_period = num_index(number_old, '.');
    int b1_len = (int)strlen(number_old);

    *int_part = 0;
    for (int i = 0; i < b1_period; ++i) {
        (*int_part) *= b1;
        (*int_part) += num_index(digits, number_old[i]);
    }

    *frac_part = 0;
    for (int i = b1_len-1; i > b1_period; --i) {
        (*frac_part) += num_index(digits, number_old[i]);
        (*frac_part) /= b1;
    }
}

int transfer_to_b2_system(char *number_new, int b2,
                          double *int_part, double *frac_part) {

    int b2_period = 0;

    while (*int_part != 0) {
        number_new[b2_period++] = digits[(int)fmod(*int_part, b2)];

        *int_part -= fmod(*int_part, b2);
        *int_part /= b2;
    }

    int b2_len = b2_period;

    while (b2_len - b2_period < 12) {
        number_new[b2_len++] = digits[(int)floor((*frac_part) * b2)];

        (*frac_part) = (*frac_part)*b2 - floor((*frac_part) * b2);
    }

    return b2_period;
}

void print_number(char *number_new, int period) {

    if (period == 0) {
        printf("0");
    }

    for (int i = period - 1; i >= 0; --i) {
        printf("%c", number_new[i]);
    }

    printf(".");

    for (int i = period; i < (period + 12); ++i) {
        printf("%c", number_new[i]);
    }
}

int main(void) {

    char number_old[64], number_new[64];

    int b1, b2;
    double int_part, frac_part;

    int variable_count = scanf("%d%d%63s", &b1, &b2, number_old);

    // Проверка входных данных
    if (!check_input(variable_count, b1, b2, number_old)) {
        printf("bad input");
        return 0;
    }

    // Дописывает ".0", если число не вещественное
    check_frac_part(number_old);

    // Перевод в десятичную систему
    transfer_to_decimal_system(number_old, b1, &int_part, &frac_part);

    // Перевод в b2-ричную систему
    int period = transfer_to_b2_system(number_new, b2, &int_part, &frac_part);

    // Вывод числа
    print_number(number_new, period);

    return 0;
}
