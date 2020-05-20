#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#define u_char unsigned char
typedef struct Tree tree;

struct Tree {
    struct Tree* left;
    struct Tree* right;

    bool is_leaf;
    u_char symbol;
    int freq;
};

tree* initializeTree();
tree* buildTree(tree*, int);
void addLeaves(tree*, u_char*, int);

void buildRestoredTree(tree*, tree*, const char*, int*, int*);

#endif //HUFFMAN_TREE_H
