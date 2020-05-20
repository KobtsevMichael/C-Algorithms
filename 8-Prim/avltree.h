#ifndef PRIM_AVLTREE_H
#define PRIM_AVLTREE_H

#include <stdlib.h>

typedef struct TreeNode {

    struct TreeNode* left;
    struct TreeNode* right;
    int height;

    int length;
    int end;

} tnode;

tnode* createNode(int, int);
tnode* insertNode(tnode**, tnode*, int, int);
int popMin(tnode**);

#endif //PRIM_AVLTREE_H
