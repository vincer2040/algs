# Algs 

algorithms and data structures based on "The Last Algorithms Course You'll Need" by
[ThePrimeagen](https://github.com/theprimeagen) on [Frontend Masters](https://frontendmasters.com/). 

you can find the full (free) course [here](https://frontendmasters.com/courses/algorithms/)

All implementations are in C99. 

## Purpose

The purpose of this project is to have reliable and fast data structures and algorithms for C programs. 

## Getting Started 

1. Clone this repo

```console
$ git clone git@github.com:vincer2040/algs.git
$ cd algs
```
2. build it

Without tests: 

```console
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make algs
```

With tests (note you must have [libcheck](https://github.com/libcheck/check) installed) 

```console
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

3. Optionally test it:

From within the build directory 

```console
$ make test
```

## Data structures included:

1. Queue
2. Stack
3. MinHeap
4. LRU
5. Vector (not in course)
6. Hashtable (not in course)
7. String (not in course)

## Algorithms included 

1. Bubble sort
2. Quick sort
3. Breadth first search on binary tree
4. In order search on binary tree
5. Post order search on binary tree
6. Pre order search on binary tree
7. Comparison of binary tree

## Testing

Tests are based on the tests provided in ThePrimeagen's [Kata-machine](https://github.com/ThePrimeagen/kata-machine)
