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

// Функция для инкремента без мьютекса
void increment_without_mutex(int id) {
    for (int i = 0; i < 1000; ++i) {
        // Изменяем общий ресурс БЕЗ синхронизации
        ++shared_data_without_mutex;

        // Выводим информацию (может быть беспорядочным из-за гонки данных)
        //std::cout << "Thread " << id << " incremented shared_data_without_mutex to " << shared_data_without_mutex << std::endl;
    }
}

// Функция для инкремента с мьютексом
void increment_with_mutex(int id) {
    for (int i = 0; i < 1000; ++i) {
        // Захватываем мьютекс перед доступом к общему ресурсу
        std::lock_guard<std::mutex> lock(mtx);

        // Изменяем общий ресурс
        ++shared_data_with_mutex;

        // Выводим информацию
        //std::cout << "Thread " << id << " incremented shared_data_with_mutex to " << shared_data_with_mutex << std::endl;
    }
}

int main() {
    // Количество потоков
    const int num_threads = 8;

    // Векторы для хранения потоков
    std::vector<std::thread> threads_without_mutex;
    std::vector<std::thread> threads_with_mutex;

    // Замер времени для инкремента без мьютекса
    auto start_without_mutex = std::chrono::high_resolution_clock::now();

    // Создаем и запускаем потоки без мьютекса
    for (int i = 0; i < num_threads; ++i) {
        threads_without_mutex.push_back(std::thread(increment_without_mutex, i));
    }

    // Ожидаем завершения всех потоков без мьютекса
    for (auto& t : threads_without_mutex) {
        t.join();
    }

    auto end_without_mutex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_without_mutex = end_without_mutex - start_without_mutex;

    // Замер времени для инкремента с мьютексом
    auto start_with_mutex = std::chrono::high_resolution_clock::now();

    // Создаем и запускаем потоки с мьютексом
    for (int i = 0; i < num_threads; ++i) {
        threads_with_mutex.push_back(std::thread(increment_with_mutex, i));
    }

    // Ожидаем завершения всех потоков с мьютексом
    for (auto& t : threads_with_mutex) {
        t.join();
    }

    auto end_with_mutex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_with_mutex = end_with_mutex - start_with_mutex;

    // Выводим итоговые значения и время выполнения
    std::cout << "\nResults:\n";
    std::cout << "Final value of shared_data_without_mutex: " << shared_data_without_mutex << std::endl;
    std::cout << "Time taken without mutex: " << elapsed_without_mutex.count() << " seconds\n";

    std::cout << "Final value of shared_data_with_mutex: " << shared_data_with_mutex << std::endl;
    std::cout << "Time taken with mutex: " << elapsed_with_mutex.count() << " seconds\n";

    return 0;
}
