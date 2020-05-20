#include "operations.h"
#include "tree.h"

#define MAX_FREQ INT_MAX

tree* initializeTree() {

    tree* nodes = malloc(511 * sizeof(tree));

    for (int i=0; i < 256; ++i) {
        nodes[i].freq = MAX_FREQ;
        nodes[i].is_leaf = false;
        nodes[i].symbol = (u_char) i;
        nodes[i].left = NULL;
        nodes[i].right = NULL;
    }

    return nodes;
}

void addLeaves(tree* nodes, u_char* buf, int size) {

    for (int i=0; i < size; ++i) {

        if (nodes[(int)buf[i]].freq == MAX_FREQ)
            nodes[(int)buf[i]].freq = 1;
        else
            nodes[(int)buf[i]].freq++ ;

        nodes[(int)buf[i]].is_leaf = true;
    }
}

tree* buildTree(tree* nodes, int size) {

    tree *min1 = (nodes[0].freq < nodes[1].freq) ? &nodes[0] : &nodes[1];
    tree *min2 = (nodes[0].freq < nodes[1].freq) ? &nodes[1] : &nodes[0];

    for (int i = 2; i < size; ++i) {
        if (nodes[i].freq < min1->freq) {
            min2 = min1;
            min1 = &nodes[i];
        }
        else if (nodes[i].freq < min2->freq) {
            min2 = &nodes[i];
        }
    }

    if (min2->freq == MAX_FREQ) {
        return min1;
    }

    nodes[size].freq = min1->freq + min2->freq;
    nodes[size].left = min2;
    nodes[size].right = min1;
    nodes[size].is_leaf = false;

    min1->freq = min2->freq = MAX_FREQ;

    return buildTree(nodes, ++size);
}

void buildRestoredTree(tree* node, tree *nodes, const char* input,
                       int *index, int *free_index) {

    if (input[*index] == '1') {

        char bin[9];

        node->is_leaf = true;
        (*index)++;

        substring(input, bin, (*index), (*index)+8);
        node->symbol = convertToChar(bin);

        node->left = NULL;
        node->right = NULL;

        (*index) += 8;
    }
    else {
        node->is_leaf = false;
        (*index)++;

        node->left = &nodes[(*free_index)++];
        buildRestoredTree(node->left, nodes, input, index, free_index);

        node->right = &nodes[(*free_index)++];
        buildRestoredTree(node->right, nodes, input, index, free_index);
    }
}
