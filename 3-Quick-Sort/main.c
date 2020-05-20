#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void swap(int *a, int *b) {

    int temp = *a;
    *a = *b;
    *b = temp;
}

void quick_sort(int *mass, int l_elem, int r_elem) {

    int i = l_elem;
    int j = r_elem;

    int base = mass[(l_elem + r_elem) / 2];

    do {
        // Сдвигаем левую границу вправо
        while (mass[i] < base) i++;

        // Сдвигаем правую границу влево
        while (mass[j] > base) j--;

        // Сортируем элементы
        if (i <= j) {
            if (mass[i] > mass[j])
                swap(&mass[i], &mass[j]);
            i++;
            j--;
        }
    } while (i <= j);

    // Рекурсивно вызываем сортировку для левой и правой части массива
    if (i < r_elem)
        quick_sort(mass, i, r_elem);
    if (j > l_elem)
        quick_sort(mass, l_elem, j);
}
 
bool read_numbers(int n, int *mass) {
 
    for (int i = 0; i < n; i++) {
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
 
    quick_sort(mass, 0, n-1);
 
    // Вывод после сортировки
    for (int i = 0; i < n; i++) {
        printf("%d ", mass[i]);
    }
    free(mass);
 
    return 0;
}
