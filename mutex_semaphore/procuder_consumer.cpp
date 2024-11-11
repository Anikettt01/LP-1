#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <unistd.h>
#include <ctime>


#define BUFF_SIZE 5 // Buffer size

struct sbuf_t {
    int buf[BUFF_SIZE];
    int in;                // Index for the next production
    int out;               // Index for the next consumption
    int count;             // Number of items in the buffer
    std::mutex mtx;        // Mutex for critical section
    std::condition_variable not_full;  // Condition variable for buffer not full
    std::condition_variable not_empty; // Condition variable for buffer not empty
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

        std::unique_lock<std::mutex> lock(shared.mtx);
        
        // Wait until buffer has space
        shared.not_full.wait(lock, [] { return shared.count < BUFF_SIZE; });

        // Insert the item into the buffer
        shared.buf[shared.in] = item;
        std::cout << "[P" << index << "] Inserted " << item << "\n";
        shared.in = (shared.in + 1) % BUFF_SIZE;
        shared.count++;

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
        if (shared.count == BUFF_SIZE) {
            std::cout << "***** Buffer is now FULL *****\n";
        }

        shared.not_empty.notify_one();
        lock.unlock();
        
        sleep(rand() % 7); // Sleep to simulate time taken to produce
    }
}

// Consumer thread function
void Consumer(int index) {
    while (true) {
        std::unique_lock<std::mutex> lock(shared.mtx);

        // Wait until buffer has at least one item
        shared.not_empty.wait(lock, [] { return shared.count > 0; });

        // Consume the item from the buffer
        int item = shared.buf[shared.out];
        shared.buf[shared.out] = -1; // Mark the slot as empty
        std::cout << "[C" << index << "] Consumed " << item << "\n";
        shared.out = (shared.out + 1) % BUFF_SIZE;
        shared.count--;

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
        if (shared.count == 0) {
            std::cout << "***** Buffer is now EMPTY *****\n";
        }

        shared.not_full.notify_one();
        lock.unlock();
        
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
    shared.in = 0;
    shared.out = 0;
    shared.count = 0;

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

    return 0;
}
