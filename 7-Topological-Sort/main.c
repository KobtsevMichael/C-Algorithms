#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void show_error(char *message) {
    FILE* out_file = fopen("out.txt", "w");
    fprintf(out_file, "%s", message);
    fclose(out_file);
    exit(0);
}

void dfs(bool** graph, char** color, int vertex, int* output,
        int *iter, int N) {

    color[vertex] = "grey";
    int i = N - 1;

    while (graph[vertex] && i >= 0) {

        if (graph[vertex][i]) {
            
            if (!strcmp(color[i], "white")) {
                dfs(graph, color, i, output, iter, N);
            }
            if (!strcmp(color[i], "grey")) {
                show_error("impossible to sort");
            }
        }

        i--;
    }

    color[vertex] = "black";
    output[(*iter)++] = vertex;
}

void topological_sort(bool** graph, int N) {

    char *color[N];
    int output[N];

    int size = 0;

    for (int i = 0; i < N; ++i)
        color[i] = "white";

    for (int i = 0; i < N; ++i) {
        if (!strcmp(color[i], "white"))
            dfs(graph, color, i, output, &size, N);
    }

    FILE* out_file = fopen("out.txt", "w");

    for (int i = size-1; i >= 0; --i) {
        fprintf(out_file, "%d ", output[i] + 1);
    }

    fclose(out_file);
}

void check_edge(int start, int end, int res, int N) {

    if ( res == EOF ) {
        show_error("bad number of lines");
    }
    if ( (start < 0) || (start > N) || (end < 0) || (end > N) ) {
        show_error("bad vertex");
    }
}

void check_for_errors(int N, int M, int res1, int res2) {

    if ( res1 == EOF || res2 == EOF ) {
        show_error("bad number of lines");
    }
    if ( (N < 0) || (N > 1000) ) {
        show_error("bad number of vertices");
    }
    if ( (M < 0) || (M > N*(N-1)/2) ) {
        show_error("bad number of edges");
    }
}

int main(void) {

    FILE* in_file = fopen("in.txt", "r");

    int N, M;

    size_t res1 = fscanf(in_file, "%d", &N);
    size_t res2 = fscanf(in_file, "%d", &M);

    check_for_errors(N, M, res1, res2);

    bool** graph = malloc(sizeof(bool*) * N);
    int start, end;

    for (int i = 0; i < N; ++i) {
        graph[i] = NULL;
    }

    for (int i = 0; i < M; ++i) {

        size_t res = fscanf(in_file, "%d%d", &start, &end);

        check_edge(start, end, res, N);

        if (!graph[start - 1]) {
            graph[start - 1] = malloc(sizeof(bool) * N);
            memset(graph[start - 1], false, N);
        }

        graph[start - 1][end - 1] = true;
    }

    fclose(in_file);

    topological_sort(graph, N);
    free(graph);

    return EXIT_SUCCESS;
}
