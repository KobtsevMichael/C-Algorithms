#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void swap(char *a, char *b) {

    char tmp = *a;
    *a = *b;
    *b = tmp;
}

char* next_permutation(char *s, int len_s) {

    char* res = "bad input";
    int i;

    for (i = len_s-1; i > 0 ; --i) {
        if (s[i-1] < s[i]) {
            int base = i;
            for (int j = base; j < len_s; ++j) {
                if (s[j] <= s[base] && s[i-1] < s[j]) {
                    base = j;
                }
            }

            swap(&s[i-1], &s[base]);

            int j = len_s - 1;
            while (i < j) {

                swap(&s[i], &s[j]);

                i++;
                j--;
            }

            res = s;
            break;
        }

        else if (s[i-1] == s[i]) break;
    }

    if (i == 0 || len_s == 1) {
        res = "";
    }

    return res;
}
 
bool check_input(char *sequence, int *num, int *leng, const char *integers) {
 
    int res = scanf("%127s%d", sequence, num);
 
    *leng = (int)strlen(sequence);
 
    for (int i = 0; i < *leng; ++i) {
        if (!strchr(integers, sequence[i]) || res != 2) {
            printf("bad input");
            return false;
        }
    }
 
    return true;
}
 
void print_permutations(char* sequence, int num, int len_seq) {
 
    for (int i = 0; i < num; ++i) {
        sequence = next_permutation(sequence, len_seq);
        printf("%s\n", sequence);
 
        if (strlen(sequence) == 0) break;
    }
 
}
 
int main(void) {
 
    const char *integers = "0123456789";
 
    char *sequence = (char *) malloc(sizeof(char) * 128);
    int num, len_seq;
 
    if (!check_input(sequence, &num, &len_seq, integers)) {
        free(sequence);
        return 0;
    }
 
    print_permutations(sequence, num, len_seq);
 
    free(sequence);
 
    return 0;
}
