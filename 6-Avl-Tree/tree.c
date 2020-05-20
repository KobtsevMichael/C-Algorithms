#include "tree.h"

int height(Tree *node) {
    return (node) ? node->height : 0;
}

int diff(Tree *node) {
    return height(node->right) - height(node->left);
}

void fix_height(Tree *node) {
    int l = height(node->left);
    int r = height(node->right);

    node->height = ((l > r) ? l : r) + 1;
}

Tree* right_rotate(Tree* a, Tree** head) {

    Tree* b = a->left;
    a->left = b->right;
    b->right = a;

    fix_height(a);
    fix_height(b);

    if ((*head) == a) (*head) = b;

    return b;
}

Tree* left_rotate(Tree* b, Tree** head) {

    Tree* a = b->right;
    b->right = a->left;
    a->left = b;

    fix_height(b);
    fix_height(a);

    if ((*head) == b) (*head) = a;

    return a;
}

Tree* big_right_rotate(Tree* node, Tree** head) {
    node->left = left_rotate(node->left, head);
    return right_rotate(node, head);
}

Tree* big_left_rotate(Tree* node, Tree** head) {
    node->right = right_rotate(node->right, head);
    return left_rotate(node, head);
}

Tree* balance(Tree* node, Tree** head) {

    fix_height(node);

    if (diff(node) == 2) {
        if (diff(node->right) < 0)
            return big_left_rotate(node, head);
        else
            return left_rotate(node, head);
    }

    if (diff(node) == -2) {
        if (diff(node->left) > 0)
            return big_right_rotate(node, head);
        else
            return right_rotate(node, head);
    }

    return node;
}

Tree* create_node(Tree* node, int val) {

    node->height = 1;
    node->value = val;

    node->left = NULL;
    node->right = NULL;

    return node;
}

Tree* add_node(Tree* node, Tree* free_node, int current_value, Tree **head) {

    if (node == NULL) {
        return create_node(free_node, current_value);
    }

    if (current_value > node->value)
        node->right = add_node(node->right, free_node, current_value, head);
    else
        node->left = add_node(node->left, free_node, current_value, head);

    return balance(node, head);
}
