#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INF LLONG_MAX

#define IN_FILE fopen("in.txt", "r")
#define OUT_FILE fopen("out.txt", "w")
#define FOR_N(i, n) for(int i=0; i < n; ++i)

#define ll_int long long int


void raiseError(char *message) {
    FILE* out_file = OUT_FILE;
    fprintf(out_file, "%s", message);
    fclose(out_file);
    exit(0);
}

void printWaysLength(ll_int* dist, int n, FILE* out_file) {

    FOR_N(i, n) {
        if (dist[i] == INF) fprintf(out_file, "oo ");
        else if (dist[i] > INT_MAX) fprintf(out_file, "INT_MAX+ ");
        else fprintf(out_file, "%lld ", dist[i]);
    }

    fprintf(out_file, "\n");
}

void printCurrentWay(ll_int* dist, int* parent, int ways_between,
                     int s, int f, FILE* out_file) {

    if (dist[f] == INF) {
        fprintf(out_file, "no path");
    }
    else if (dist[f] > INT_MAX && ways_between >= 2) {
        fprintf(out_file, "overflow");
    }
    else {
        for (int vertex = f; vertex != s; vertex = parent[vertex])
            fprintf(out_file, "%d ", vertex + 1);

        fprintf(out_file, "%d ", s + 1);
    }
}

void countMinWaysLength(int** graph_matrix, int n, int s, int f) {

    ll_int dist[n];
    int parent[n], ways_between = 0;
    bool visited[n];

    FOR_N(i, n) {
        dist[i] = INF;
        parent[i] = EOF;
        visited[i] = false;
    }

    dist[s] = 0;

    FOR_N(i, n) {

        ll_int min_dist = INF;
        int min_vertex = EOF;

        FOR_N(j, n) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                min_vertex = j;
            }
        }

        if (min_dist == INF) {
            break;
        }

        visited[min_vertex] = true;

        FOR_N(k, n) {

            int len = graph_matrix[min_vertex][k];

            if (len != EOF && dist[min_vertex] + len < dist[k]) {
                dist[k] = dist[min_vertex] + len;
                parent[k] = min_vertex;
                ways_between += (min_vertex == s);
            }
        }
    }

    FILE* out_file = OUT_FILE;

    printWaysLength(dist, n, out_file);
    printCurrentWay(dist, parent, ways_between, s, f, out_file);

    fclose(out_file);
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

void checkGraphInfo(int n, int s, int f, int m, int res) {
    if (res == EOF) {
        raiseError("bad number of lines");
    }
    if (n < 0 || n > 5000) {
        raiseError("bad number of vertices");
    }
    if (m < 0 || m > n*(n-1)/2) {
        raiseError("bad number of edges");
    }
    if (s < 1 || s > n || f < 1 || f > n) {
        raiseError("bad vertex");
    }
}

int main(void) {

    FILE *in_file = IN_FILE;

    int **graph_matrix, n, s, f, m, e_start, e_end;
    ll_int  e_length;

    size_t res = fscanf(in_file, "%d\n%d%d\n%d", &n, &s, &f, &m);

    checkGraphInfo(n, s, f, m, res);

    graph_matrix = malloc(sizeof(int *) * n);

    FOR_N(i, n) {
        graph_matrix[i] = malloc(sizeof(int) * n);
        memset(graph_matrix[i], EOF, sizeof(int) * n);
    }

    FOR_N(i, m) {
        res = fscanf(in_file, "%d%d%lld", &e_start, &e_end, &e_length);
        checkEdge(e_start, e_end, e_length, res, n);

        graph_matrix[e_start - 1][e_end - 1] = (int)e_length;
        graph_matrix[e_end - 1][e_start - 1] = (int)e_length;
    }

    countMinWaysLength(graph_matrix, n, --s, --f);

    free(graph_matrix);
    fclose(in_file);

    return 0;
}
