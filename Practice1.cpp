#include <iostream>
#include <thread>
#include <chrono>

// Функция для однопотока
unsigned long long factorial_single_thread(unsigned long long n) {
    unsigned long long result = 1;
    for (unsigned long long i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}


void partial_factorial(unsigned long long start, unsigned long long end, unsigned long long &result) {
    unsigned long long temp = 1;
    for (unsigned long long i = start; i <= end; ++i) {
        temp *= i;
    }
    result = temp;
}

// Функция для двупотока
unsigned long long factorial_two_threads(unsigned long long n) {
    unsigned long long result1 = 1, result2 = 1;
    unsigned long long mid = n / 2;

    std::thread t1(partial_factorial, 1, mid, std::ref(result1));
    std::thread t2(partial_factorial, mid + 1, n, std::ref(result2));

    t1.join();
    t2.join();

    return result1 * result2;
}

int main() {
    unsigned long long n = 10;

    // Однопоток
    auto start_single = std::chrono::high_resolution_clock::now();
    unsigned long long result_single = factorial_single_thread(n);
    auto end_single = std::chrono::high_resolution_clock::now();
    auto duration_single = std::chrono::duration_cast<std::chrono::nanoseconds>(end_single - start_single);

    // Двухпоток
    auto start_two = std::chrono::high_resolution_clock::now();
    unsigned long long result_two = factorial_two_threads(n);
    auto end_two = std::chrono::high_resolution_clock::now();
    auto duration_two = std::chrono::duration_cast<std::chrono::nanoseconds>(end_two - start_two);

    
    std::cout << "factorial " << n << " (1 thread) = " << result_single << ", time: " << duration_single.count() << " ns" << std::endl;
    std::cout << "factorial " << n << " (2 thread) = " << result_two << ", time: " << duration_two.count() << " ns" << std::endl;

    return 0;
}

