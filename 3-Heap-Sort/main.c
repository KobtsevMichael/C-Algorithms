#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void swap(int *a, int *b) {

    int temp = *a;
    *a = *b;
    *b = temp;
}

// Просеивание через кучу
void sift(int* mass, int base, int child) {

    int max_elem;
    bool finish = false;

    while ((!finish) && (base * 2 <= child)) {

        if (base * 2 == child)
            max_elem = base * 2;
        else if (mass[base * 2] > mass[base * 2 + 1])
            max_elem = base * 2;
        else
            max_elem = base * 2 + 1;

        if (mass[base] < mass[max_elem]) {
            swap(&mass[base], &mass[max_elem]);
            base = max_elem;
        }
        else {
            finish = true;
        }
    }
}

void heap_sort(int n, int* mass) {

    // Нижний ряд пирамиды
    for (int i = n/2; i >= 0; --i) {
        sift(mass, i, n-1);
    }

    // Просеивание остальных элементов
    for (int i = n - 1; i > 0; --i) {
        swap(&mass[0], &mass[i]);
        sift(mass, 0, i-1);
    }
}

bool read_numbers(int n, int* mass) {
 
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &mass[i]) == EOF)
            return false;
    }
 
    return true;
}
 
int main(void) {
 
    int n;
    
    if (scanf("%d", &n) == EOF) {
        return 0;
    }
    if (n <= 0) {
        return 0;
    }
    
    int *mass = (int *)malloc(sizeof(int) * n);
 
    if (!read_numbers(n, mass)) {
        free(mass);
        return 0;
    }
 
    heap_sort(n, mass);
 
    // Вывод после сортировки
    for (int i = 0; i < n; ++i) {
        printf("%d ", mass[i]);
    }
    free(mass);
 
    return 0;
}
