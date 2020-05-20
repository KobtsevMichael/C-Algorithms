#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "qsort.h"

#define IN_FILE fopen("in.txt", "r")
#define OUT_FILE fopen("out.txt", "w")

void raiseError(char *message) {
    FILE* out_file = OUT_FILE;
    fprintf(out_file, "%s", message);
    fclose(out_file);
    exit(0);
}

void printFrame(edge* graph_edges, const int *min_frame, int index) {

    FILE* out_file = OUT_FILE;

    for (int i=0; i < index; ++i) {
        fprintf(out_file, "%d %d\n",
                graph_edges[min_frame[i]].start + 1,
                graph_edges[min_frame[i]].end + 1);
    }

    fclose(out_file);
}

void searchMinFrame(edge* graph_edges, int n, int m) {

    int min_frame[n], tree_id[n], index = 0;

    quickSort(graph_edges, 0, m-1);

    for (int i=0; i < n; ++i) {
        tree_id[i] = i;
    }

    for (int i=0; i < m; ++i) {
        int a = graph_edges[i].start;
        int b = graph_edges[i].end;

        bool isAB = 0 <= a && a <= n && 0 <= b && b <= n;

        if (isAB && tree_id[a] != tree_id[b]) {

            int old_id = tree_id[b];

            for (int j=0; j < n; ++j) {
                if (tree_id[j] == old_id) {
                    tree_id[j] = tree_id[a];
                }
            }

            min_frame[index++] = i;
        }
    }

    if (index != n-1) {
        raiseError("no spanning tree");
    }

    printFrame(graph_edges, min_frame, index);
}

void checkEdge(edge edge, int res, int n) {
    if (res == EOF) {
        raiseError("bad number of lines");
    }
    if (edge.start < 1 || edge.start > n || edge.end < 1 || edge.end > n) {
        raiseError("bad vertex");
    }
    if (edge.length < 0 || edge.length > INT_MAX) {
        raiseError("bad length");
    }
}

void checkGraphSize(int n, int m, int res) {
    if (res == EOF) {
        raiseError("bad number of lines");
    }
    if (n < 0 || n > 5000) {
        raiseError("bad number of vertices");
    }
    if (m < 0 || m > n*(n-1)/2) {
        raiseError("bad number of edges");
    }
    if (m < n-1 || (m == 0 && n != 1)) {
        raiseError("no spanning tree");
    }
}

int main(void) {

    FILE* in_file = IN_FILE;

    edge *graph_edges;
    int n, m;

    size_t res = fscanf(in_file, "%d\n%d", &n, &m);

    checkGraphSize(n, m, res);

    graph_edges = malloc(sizeof(edge) * (m+1));

    for (int i=0; i < m; ++i) {

        res = fscanf(in_file, "%d%d%lld",
                     &graph_edges[i].start,
                     &graph_edges[i].end,
                     &graph_edges[i].length);

        checkEdge(graph_edges[i], res, n);

        graph_edges[i].start-- ;
        graph_edges[i].end-- ;
    }

    searchMinFrame(graph_edges, n, m);

    free(graph_edges);
    fclose(in_file);

    return 0;
}
