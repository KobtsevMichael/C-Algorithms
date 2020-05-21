#include <stdio.h>
#include <string.h>
#include <stdbool.h>
 
#define PATTERN_SIZE (18)
#define BUF_SIZE (4000)

void check_end_file(unsigned char *text, unsigned const char *buf,
                    int *index, int *k, bool *end_file, int res) {

    if (buf[*k] == '\0' || res == EOF) {
        *end_file = true;
    }
    else {
        (*index)++;
        (*k)++;

        text[(*index) % BUF_SIZE] = buf[(*k) - 1];
    }
}


void bm_search(unsigned const char *pattern, unsigned char *text,
               int len_p, const int *stop) {

    unsigned char buf[BUF_SIZE];
    int index = len_p - 1;

    int k = BUF_SIZE;
    int res = 0;

    bool end_file = false;
    while (!end_file) {

        // Рассчет сдвига
        int shift = stop[(short)text[index % BUF_SIZE]];

        // Считывание n элементов из буфера, где shift - длина сдвига
        for (int j = 0; j < shift; ++j) {

            if (k == BUF_SIZE) {
                memset(buf, 0, sizeof(buf));
                res = (int) fread(buf, sizeof(char), BUF_SIZE, stdin);
                k = 0;
            }

            // Проверка на конец файла
            check_end_file(text, buf, &index, &k, &end_file, res);
        }

        if (end_file) continue;

        // Сравнение элементов строки и подстроки
        for (int m = index; m > index - len_p; --m) {

            printf("%d ", m + 1);

            if (text[m % BUF_SIZE] != pattern[len_p - index + m - 1]) {
                break;
            }
        }
    }
}

void create_stop_array(int length, int *arr) {

    for (int i = 0; i < 256; ++i) {
        arr[i] = length;
    }
}

void filling_of_stop_array(unsigned const char *p, int length, int *arr) {

    for (int i = 0; i < length-1; ++i) {
        arr[(short) p[i]] = length - i - 1;
    }
}

int read_substring(unsigned char *pattern) {
 
    int len_p;
 
    for (len_p = 0; len_p < PATTERN_SIZE-1; ++len_p) {

        if (fscanf(stdin, "%c", &pattern[len_p]) == EOF) {
            return 0;
        }
        if (pattern[len_p] == '\n') {
            break;
        }
    }
    pattern[len_p] = 0;
 
    return len_p;
}
 
bool read_text(unsigned char *text, const unsigned char *pattern, int len_p) {
 
    if ((int)fread(text, sizeof(char), len_p, stdin) != len_p) {
        return false;
    }
 
    int i = len_p;
 
    do {
        printf("%d ", i);
        i--;
    } while (i > 0 && text[i] == pattern[i]);
 
    return true;
}
 
int main(void) {
 
    unsigned char pattern[PATTERN_SIZE];
    unsigned char text[BUF_SIZE];
 
    // Считывание подстроки
    int len_p = read_substring(pattern);
 
    if (len_p == 0) {
        return 0;
    }
 
    // Считывание первых символов текста
    if (!read_text(text, pattern, len_p)) {
        return 0;
    }
 
    // Создание массива со сдвигами для стоп символов
    int stop_arr[256];
 
    create_stop_array(len_p, stop_arr);
    filling_of_stop_array(pattern, len_p, stop_arr);
 
    // Поиск
    bm_search(pattern, text, len_p, stop_arr);
 
    return 0;
}
