#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H

#include <stdlib.h>

typedef struct Tree {
    struct Tree* left;
    struct Tree* right;
    int height;
    int value;
} Tree;

Tree* create_node(Tree*, int);
Tree* add_node(Tree*, Tree*, int, Tree**);

#endif //AVL_TREE_AVLTREE_H
