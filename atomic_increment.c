#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 2
#define NUM_INCREMENTS 1000000

atomic_int counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        atomic_fetch_add(&counter, 1);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    clock_t start, end;

    start = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    end = clock();

    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;


    printf("Final counter value: %d\n", counter);
    printf("Elapsed time: %f seconds\n", elapsed_time);

    return 0;
}
