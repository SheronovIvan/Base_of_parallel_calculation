#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

// Общий ресурс
int shared_data_without_mutex = 0;
int shared_data_with_mutex = 0;

// Мьютекс для синхронизации доступа к shared_data_with_mutex
std::mutex mtx;

// Функция без мьютекса
void increment_without_mutex(int id) {
    for (int i = 0; i < 100000; ++i) {
        ++shared_data_without_mutex;
    }
}

// Функция с мьютексом
void increment_with_mutex(int id) {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++shared_data_with_mutex;
    }
}

int main() {
    const int num_threads = 10;


    std::vector<std::thread> threads_without_mutex;
    std::vector<std::thread> threads_with_mutex;


    auto start_without_mutex = std::chrono::high_resolution_clock::now();

  
    for (int i = 0; i < num_threads; ++i) {
        threads_without_mutex.push_back(std::thread(increment_without_mutex, i));
    }

  
    for (auto& t : threads_without_mutex) {
        t.join();
    }

    auto end_without_mutex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_without_mutex = end_without_mutex - start_without_mutex;

  
    auto start_with_mutex = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < num_threads; ++i) {
        threads_with_mutex.push_back(std::thread(increment_with_mutex, i));
    }

  
    for (auto& t : threads_with_mutex) {
        t.join();
    }

    auto end_with_mutex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_with_mutex = end_with_mutex - start_with_mutex;


    std::cout << "\nResults:\n";
    std::cout << "Final value of shared_data_without_mutex: " << shared_data_without_mutex << std::endl;
    std::cout << "Time taken without mutex: " << elapsed_without_mutex.count() << " seconds\n";

    std::cout << "Final value of shared_data_with_mutex: " << shared_data_with_mutex << std::endl;
    std::cout << "Time taken with mutex: " << elapsed_with_mutex.count() << " seconds\n";

    return 0;
}
