# Topological Sort

Реализация алгоритма топологической сортировки графа

### Input

```
Строка 1: Целое N от 0 до 1000 - число вершин в графе
```
```
Строка 2: Целое M от 0 до N*(N-1)/2 - число рёбер в графе
```
```
Строка 3, ..., M+2: Начало и конец ребра
```

### Output

```
Если N не от 0 до 1000, то "bad number of vertices"
Если M не от 0 до N*(N-1)/2, то "bad number of edges"
Если номер вершины не от 1 до N, то "bad vertex"
Если строк меньше M+2, то "bad number of lines"
```
```
Если граф не допускает топологическую сорировку, то "impossible to sort"
```
```
Если граф допускает топологическую сортировку, то вершины графа после
топологической сортировки
```

### Other information

```
Time limit: 3 seconds
```
```
Memory limit: (sizeof(void*)+sizeof(void*))*M + 1024Kb
```