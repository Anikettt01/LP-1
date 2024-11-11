#include <iostream>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <mutex>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#define BUFF_SIZE 5 // Buffer size

struct sbuf_t {
    int buf[BUFF_SIZE];
    int in;                // Index for the next production
    int out;               // Index for the next consumption
    sem_t full;            // Semaphore to track full slots
    sem_t empty;           // Semaphore to track empty slots
    std::mutex mtx;        // Mutex for critical section
};

sbuf_t shared;

// Initialize the buffer with -1 to indicate empty slots
void initialize_buffer() {
    for (int i = 0; i < BUFF_SIZE; i++) {
        shared.buf[i] = -1; // Mark each slot as empty initially
    }
}

// Producer thread function
void Producer(int index) {
    while (true) {
        int item = rand() % 10; // Generate a random item

        // Wait for an empty slot
        sem_wait(&shared.empty);
        std::lock_guard<std::mutex> lock(shared.mtx);

        // Insert the item into the buffer
        shared.buf[shared.in] = item;
        std::cout << "[P" << index << "] Inserted " << item << "\n";
        shared.in = (shared.in + 1) % BUFF_SIZE;

        // Display the current state of the buffer
        std::cout << "Buffer: ";
        for (int i = 0; i < BUFF_SIZE; i++) {
            if (shared.buf[i] == -1)
                std::cout << "_ "; // Display '_' for empty slots
            else
                std::cout << shared.buf[i] << " ";
        }
        std::cout << "\n";

        // Check if the buffer is full
        if (shared.in == shared.out) {
            std::cout << "***** Buffer is now FULL *****\n";
        }

        sem_post(&shared.full);
        sleep(rand() % 7); // Sleep to simulate time taken to produce
    }
}

// Consumer thread function
void Consumer(int index) {
    while (true) {
        // Wait for a full slot
        sem_wait(&shared.full);
        std::lock_guard<std::mutex> lock(shared.mtx);

        // Consume the item from the buffer
        int item = shared.buf[shared.out];
        shared.buf[shared.out] = -1; // Mark the slot as empty
        std::cout << "[C" << index << "] Consumed " << item << "\n";
        shared.out = (shared.out + 1) % BUFF_SIZE;

        // Display the current state of the buffer
        std::cout << "Buffer: ";
        for (int i = 0; i < BUFF_SIZE; i++) {
            if (shared.buf[i] == -1)
                std::cout << "_ "; // Display '_' for empty slots
            else
                std::cout << shared.buf[i] << " ";
        }
        std::cout << "\n";

        // Check if the buffer is empty
        if (shared.in == shared.out) {
            std::cout << "***** Buffer is now EMPTY *****\n";
        }

        sem_post(&shared.empty);
        sleep(rand() % 7); // Sleep to simulate time taken to consume
    }
}

int main() {
    int NP, NC;
    srand(time(NULL));

    // Input number of producers and consumers
    std::cout << "Enter number of producers: ";
    std::cin >> NP;
    std::cout << "Enter number of consumers: ";
    std::cin >> NC;

    // Initialize buffer
    initialize_buffer();

    // Initialize indices and semaphores
    shared.in = 0;
    shared.out = 0;
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);

    // Create producer and consumer threads
    std::vector<std::thread> producers, consumers;

    for (int i = 0; i < NP; i++) {
        producers.emplace_back(Producer, i);
    }
    for (int i = 0; i < NC; i++) {
        consumers.emplace_back(Consumer, i);
    }

    // Wait for all producer threads to finish
    for (auto &p : producers) {
        if (p.joinable()) {
            p.join();
        }
    }

    // Wait for all consumer threads to finish
    for (auto &c : consumers) {
        if (c.joinable()) {
            c.join();
        }
    }

    // Cleanup
    sem_destroy(&shared.full);
    sem_destroy(&shared.empty);

    return 0;
}
