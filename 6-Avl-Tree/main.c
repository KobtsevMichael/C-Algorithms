#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

int tree_height_count(FILE* in_file) {

    int N, value;

    if (fscanf(in_file, "%d", &N) == EOF) {
        exit(0);
    }

    if (N == 0) {
        return 0;
    }

    if (fscanf(in_file, "%d", &value) == EOF) {
        exit(0);
    }

    Tree* nodes = (Tree *)malloc(sizeof(Tree) * N);
    Tree* head = &nodes[0];

    create_node(head, value);

    for (int i = 1; i < N; i++) {
        if (fscanf(in_file, "%d", &value) == EOF) {
            exit(0);
        }

        add_node(head, &nodes[i], value, &head);
    }

    return head->height;
}

int main(void) {

    FILE* in_file = fopen("in.txt", "r");
    FILE* out_file = fopen("out.txt", "w");

    int h = tree_height_count(in_file);

    fprintf(out_file, "%d", h);

    fclose(in_file);
    fclose(out_file);

    return 0;
}
