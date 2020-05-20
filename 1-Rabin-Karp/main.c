#include <stdio.h>
#include <stdbool.h>
#include <string.h>
 
#define PATTERN_SIZE (20)
#define BUF_SIZE (20)

int power(int num, int degree) {

    int res = 1;
    for (int i = 0; i < degree; ++i) {
        res *= num;
    }
    return res;
}

int hash_func(unsigned const char *string, int len_s) {

    int res = 0;
    for (int i = 0; i < len_s; i++) {
        res += ((short)string[i] % 3) * power(3, i);
    }

    return res;
}

void update_hash(unsigned char *txt, const unsigned char *buf,
                 int *hash_t, int k, int length, int const_i) {

    *hash_t -= (int)txt[0] % 3;
    *hash_t /= 3;

    memmove(txt, &txt[1], length);

    txt[length - 1] = buf[k];

    *hash_t += ((short)txt[length-1] % 3) * const_i;

}

bool check_end_file(const unsigned char *txt, int *index, int *k,
                    bool *end_file, int leng, int res) {

    if (txt[leng - 1] == '\0' || res == EOF) {

        *end_file = true;
        return true;
    }
    else {
        (*index)++;
        (*k)++;

        return false;
    }
}
 
void rc_search(unsigned char *txt, unsigned const char *p,
               int hash_t, int hash_p, int leng) {

    unsigned char buf[BUF_SIZE];

    int index = 0;
    int const_i = power(3, leng - 1);

    int k = BUF_SIZE;
    int res = 0;

    bool end_file = false;
    while (!end_file) {

        // Считывание элементов в буфер
        if (k == BUF_SIZE) {
            memset(buf, 0, sizeof(buf));
            res = (int)fread(buf, sizeof(char), BUF_SIZE, stdin);
            k = 0;
        }

        // Обновление текста и подсчет нового значения хэш функции
        update_hash(txt, buf, &hash_t, k, leng, const_i);

        // Проверка на конец файла и ошибку чтения
        if (check_end_file(txt, &index, &k, &end_file, leng, res))
            continue;

        // Если совпадают значения хэш функций, то сравнивем элементы
        if (hash_t == hash_p)
            for (int i = 0; i < leng; ++i) {
                printf("%d ", index + i + 1);
                if (txt[i] != p[i]) break;
            }
    }
}
 
int read_substring(unsigned char *pattern) {
 
    int len_p;
 
    for (len_p = 0; len_p < PATTERN_SIZE - 1; ++len_p) {
 
        if (fscanf(stdin, "%c", &pattern[len_p]) == EOF) {
            return 0;
        }
        if (pattern[len_p] == '\n') {
            break;
        }
    }
    pattern[len_p] = '\0';
 
    return len_p;
}
 
bool read_text(unsigned char *text, const unsigned char *pattern,
        int len_p, int pattern_hash, int *text_hash) {
 
    if ((int)fread(text, sizeof(char), len_p, stdin) != len_p) {
        return false;
    }
 
    *text_hash = hash_func(text, len_p);
 
    if (*text_hash == pattern_hash) {
        for (int i = 0; i < len_p; ++i) {
            printf("%d ", i + 1);
            if (pattern[i] != text[i]) break;
        }
    }
 
    return true;
}
 
int main() {
 
    unsigned char pattern[PATTERN_SIZE];
    unsigned char text[BUF_SIZE];
 
    // Считывание подстроки
    int len_p = read_substring(pattern);
 
    // Подсчёт хэша для подстроки и текста
    int pattern_hash = hash_func(pattern, len_p);
    int text_hash;
 
    printf("%d ", pattern_hash);
 
    // Считывание первых len_p символов текста
    if (!read_text(text, pattern, len_p, pattern_hash, &text_hash)) {
        return 0;
    }
 
    // Поиск
    rc_search(text, pattern, text_hash, pattern_hash, len_p);
 
    return 0;
}
