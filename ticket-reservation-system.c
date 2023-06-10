#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 5
#define MAX_SEATS 10
int availableSeats = MAX_SEATS;
pthread_mutex_t mutex;
void* reserveTicket(void* threadId) {
    int* tid = (int*)threadId;
    int allocatedSeat = -1;  // Variable to store the allocated seat number
    // Simulate some work being done by the thread
    // Sleep for a random duration to simulate different users reserving tickets
    int sleepTime = rand() % 5 + 1;
    printf("Thread %d is reserving a ticket...\n", *tid);
    sleep(sleepTime);

    // Reserve the ticket
    pthread_mutex_lock(&mutex);
    if (availableSeats > 0) {
        availableSeats--;
        allocatedSeat = availableSeats + 1;  // Seat number starts from 1
        printf("Thread %d reserved seat %d. %d seats remaining.\n", *tid, allocatedSeat, availableSeats);
    } else {
        printf("Thread %d couldn't reserve a ticket. No seats available.\n", *tid);
    }
    pthread_mutex_unlock(&mutex);
    // Pass the allocated seat number back to the main thread
    pthread_exit((void*)allocatedSeat);
}
int main() {
    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];
    void* threadResult[NUM_THREADS];
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL)
    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadIds[i] = i;
        int result = pthread_create(&threads[i], NULL, reserveTicket, (void*)&threadIds[i]);
        if (result != 0) {
            printf("Error creating thread %d. Exiting.\n", i);
            exit(-1);
        }
    }
    // Wait for all threads to finish and collect their results
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &threadResult[i]);
    }
    // Print the allocated seats
    printf("Allocated seats:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        int seatNumber = (int)threadResult[i];
        if (seatNumber != -1) {
            printf("Thread %d: Seat %d\n", i, seatNumber);
        }
    }
    // Print the final result
    printf("Final available seats: %d\n", availableSeats);
    // Clean up the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
