#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "avltree.h"

#define IN_FILE fopen("in.txt", "r")
#define OUT_FILE fopen("out.txt", "w")

#define ll_int long long int


void raiseError(char *message) {
    FILE* out_file = OUT_FILE;
    fprintf(out_file, "%s", message);
    fclose(out_file);
    exit(0);
}

void printFrame(int *min_frame, int size) {

    FILE* out_file = OUT_FILE;

    for (int i=0; i < size; i += 2) {
        fprintf(out_file, "%d %d\n",
                min_frame[i] + 1,
                min_frame[i + 1] + 1);
    }

    fclose(out_file);
}

void searchMinFrame(int** graph, int n) {

    int frame[2*n], frame_pos = 0, ends[n], min[n];
    bool visited[n];

    for (int i = 0; i < n; ++i) {
        min[i] = -1;
        ends[i] = -1;
        visited[i] = 0;
    }

    tnode* edges = createNode(0, 0);

    while (edges != NULL) {

        int min_vertex = popMin(&edges);

        if (visited[min_vertex]) {
            continue;
        }

        visited[min_vertex] = 1;

        if (ends[min_vertex] >= 0) {
            frame[frame_pos++] = min_vertex;
            frame[frame_pos++] = ends[min_vertex];
        }

        for (int j = 0; j < n; ++j) {

            int len = graph[min_vertex][j];

            if (!visited[j] && len > 0 && (len < min[j] || min[j] < 0)) {
                insertNode(&edges, edges, len, j);
                min[j] = len;
                ends[j] = min_vertex;
            }
        }
    }

    free(edges);

    if (frame_pos != 2*(n-1)) {
        raiseError("no spanning tree");
    }

    printFrame(frame, frame_pos);
}

void checkEdge(int start, int end, ll_int len, int res, int n) {

    if (res == EOF) {
        raiseError("bad number of lines");
    }
    if (start < 1 || start > n || end < 1 || end > n) {
        raiseError("bad vertex");
    }
    if (len < 0 || len > INT_MAX) {
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

    FILE *in_file = IN_FILE;

    int **graph, n, m, e_start, e_end;
    ll_int e_length;

    size_t res = fscanf(in_file, "%d\n%d", &n, &m);
    checkGraphSize(n, m, res);

    graph = malloc(n * sizeof(int *));

    for (int i=0; i < n; ++i) {
        graph[i] = malloc(n * sizeof(int));
        memset(graph[i], -1, n * sizeof(int));
    }

    for (int i=0; i < m; ++i) {
        res = fscanf(in_file, "%d%d%lld", &e_start, &e_end, &e_length);
        checkEdge(e_start, e_end, e_length, res, n);

        graph[e_start - 1][e_end - 1] = (int)e_length;
        graph[e_end - 1][e_start - 1] = (int )e_length;
    }

    searchMinFrame(graph, n);

    free(graph);
    fclose(in_file);

    return 0;
}
