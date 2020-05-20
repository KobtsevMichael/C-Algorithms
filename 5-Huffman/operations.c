#include "operations.h"

char* convertToBin(u_char a) {

    static char bin[9] = {0};

    for (int i = 7; i >= 0; --i) {
        bin[i] = (a % 2) ? '1' : '0';
        a /= 2;
    }
    return bin;
}

u_char convertToChar(const char *bin) {

    int res = 0;

    for (int i = 0; i < 8; ++i) {
        if (bin[i] == '1') {
            res += (int) pow(2, 7 - i);
        }
    }
    return (u_char) res;
}

void substring(const char* str, char* sub, int start, int end) {

    int i, j;

    for (i = start, j = 0; i < end; ++i, ++j) {
        sub[j] = str[i];
    }
    sub[j] = 0;
}

void copyChar(char* to, char* from) {

    for (size_t i=0; i < strlen(from); ++i) {
        to[i] = from[i];
    }
    for (size_t i=strlen(from); i < strlen(to); ++i) {
        to[i] = 0;
    }
}

