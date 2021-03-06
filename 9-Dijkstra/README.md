# Dijkstra's Algorithm

Реализация алгоритма Дейкстры для нахождения кратчайшего пути в графе

### Input

```
Строка 1: Целое N от 0 до 5000 - число вершин в графе
```
```
Строка 2: Целое S и целое F от 1 до N - начало и конец пути
```
```
Строка 3: Целое M от 0 до N*(N-1)/2 - число рёбер в графе
```
```
Строка 4, ..., M+3: Начало, конец и длина ребра
```

### Output

```
Если N не от 0 до 5000, то "bad number of vertices"
Если M не от 0 до N*(N-1)/2, то "bad number of edges"
Если номер вершины не от 1 до N, то "bad vertex"
Если длина ребра не от 0 до INT_MAX, то "bad length"
Если строк меньше M+3, то "bad number of lines"
```
```
Иначе в 1 строке: N значений - расстояния от S до каждой вершины графа

> oo			   | Если нет пути
> INT_MAX+		   | Если длина кратчайшего пути > INT_MAX
> Длина кратчайшего пути   | Если длина кратчайшего пути <= INT_MAX

Во 2 строке: Информация о вершинах кратчайшего пути от S до F

> no path		   | Если нет пути
> overflow                 | Если длина кратчайшего пути > INT_MAX
> Вершины кратчайшего пути | Если длина кратчайшего пути <= INT_MAX

```

### Other information

```
Time limit: 3 seconds
```
```
Memory limit: 4*N*N + 1024Kb
```
