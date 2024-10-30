#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 1000000
#define ITERATIONS 1000


void sequential_access(int *array){
    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < ARRAY_SIZE; j++) {
            volatile int a = array[j];
        }
    }
}

void random_access(int *array, int *indices) {
    for (int i = 0; i < ITERATIONS; ++i) {
        for (int j = 0; j < ARRAY_SIZE; ++j) {
            array[indices[j]]++;
        }
    }
}

void scattered_access(int *array) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
        for (int i = 0; i < ITERATIONS; i++) {
            volatile int a = array[j];
        }
    }
}

void shuffle_indices(int *indices, int size) {
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}

int main() {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    for (int i = 0; i < ARRAY_SIZE; i++) array[i] = 0;
    int *indices = (int *)malloc(ARRAY_SIZE * sizeof(int));
    for (int i = 0; i < ARRAY_SIZE; i++) indices[i] = i;

    shuffle_indices(indices, ARRAY_SIZE);

    clock_t start, end;

    start = clock();
    sequential_access(array);
    end = clock();
    printf("Sequential access time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    for (int i = 0; i < ARRAY_SIZE; i++) array[i] = 0;

    start = clock();
    random_access(array, indices);
    end = clock();
    printf("Random access time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    for (int i = 0; i < ARRAY_SIZE; i++) array[i] = 0;

    start = clock();
    scattered_access(array);
    end = clock();
    printf("Scattered access time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(array);
    return 0;
}
