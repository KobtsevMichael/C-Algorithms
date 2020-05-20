#include "qsort.h"

void swap(edge *a, edge *b) {
    edge temp = *a;
    *a = *b;
    *b = temp;
}

int sort(edge* mass, int l, int r) {

    edge center = mass[(l + r) / 2];
    int i = l, j = r;

    while (i <= j) {
        while (mass[i].length < center.length) {
            i++;
        }
        while (mass[j].length > center.length) {
            j--;
        }
        if (i >= j) {
            break;
        }
        swap(&mass[i], &mass[j]);
        i++;
        j--;
    }
    return j;
}

void quickSort (edge* mass, int l, int r) {
    if (l < r) {
        int q = sort(mass, l, r);
        quickSort(mass, l, q);
        quickSort(mass, q + 1, r);
    }
}
