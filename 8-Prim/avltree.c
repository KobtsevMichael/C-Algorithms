#include "avltree.h"

int getHeight(tnode* node) {
    return node ? node->height : 0;
}

int diff(tnode* node) {
    return getHeight(node->right) - getHeight(node->left);
}

void fixHeight(tnode* node) {
    int l = getHeight(node->left);
    int r = getHeight(node->right);

    node->height = ((l > r) ? l : r) + 1;
}

tnode* rightRotation(tnode** head, tnode* p) {
    tnode* q = p->left;
    p->left = q->right;
    q->right = p;

    fixHeight(p);
    fixHeight(q);
    if (p == *head) *head = q;

    return q;
}

tnode* leftRotation(tnode** head, tnode* q) {
    tnode* p = q->right;
    q->right = p->left;
    p->left = q;

    fixHeight(q);
    fixHeight(p);
    if (q == *head) *head = p;

    return p;
}

tnode* balance(tnode** head, tnode* node) {

    fixHeight(node);

    if (diff(node) == 2) {
        if (diff(node->right) < 0)
            node->right = rightRotation(head, node->right);
        return leftRotation(head, node);
    }
    if (diff(node) == -2) {
        if (diff(node->left) > 0)
            node->left = leftRotation(head, node->left);
        return rightRotation(head, node);
    }

    return node;
}

tnode* createNode(int length, int end) {

    tnode* node = malloc(sizeof(tnode));

    node->length = length;
    node->end = end;
    node->height = 1;
    node->left = node->right = NULL;

    return node;
}

tnode* insertNode(tnode** head, tnode* node, int length, int end) {

    if (*head == NULL) {
        *head = createNode(length, end);
        return *head;
    }
    if (node == NULL) {
        return createNode(length, end);
    }

    if (length < node->length)
        node->left = insertNode(head, node->left, length, end);
    else
        node->right = insertNode(head, node->right, length, end);

    return balance(head, node);
}

tnode* removeMin(tnode** head, tnode* node, int* min) {

    if (node->left == NULL) {

        *min = node->end;

        tnode* right = node->right;
        free(node);

        return right;
    }

    node->left = removeMin(head, node->left, min);
    return balance(head, node);
}

int popMin(tnode** head) {

    if ((*head)->left == NULL) {

        int min = (*head)->end;
        *head = (*head)->right;

        return min;
    }

    int min;
    removeMin(head, *head, &min);

    return min;
}
