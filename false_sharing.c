#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NUM_THREADS 2
#define NUM_INCREMENTS 1000000

atomic_int counters[NUM_THREADS];

void* increment_counter(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        atomic_fetch_add(&counters[thread_id], 1);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        atomic_init(&counters[i], 0);
        thread_ids[i] = i;
    }

    clock_t start, end;
    start = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    end = clock();

    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_count += counters[i];
    }

    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Total counter value: %d\n", total_count);
    printf("Elapsed time: %f seconds\n", elapsed_time);

    return 0;
}
