#ifndef KRUSKAL_QSORT_H
#define KRUSKAL_QSORT_H

typedef struct Edge {
    int start;
    int end;
    long long int length;
} edge;

void quickSort (edge*, int, int);

#endif //KRUSKAL_QSORT_H
