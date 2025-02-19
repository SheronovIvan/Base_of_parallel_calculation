#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace std;


std::vector<std::vector<int>> matrixMultiplyStatic(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();

    std::vector<std::vector<int>> C(n, std::vector<int>(p, 0));

#pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

std::vector<std::vector<int>> matrixMultiplyDynamic(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();

    std::vector<std::vector<int>> C(n, std::vector<int>(p, 0));

#pragma omp parallel for schedule(dynamic,4)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

std::vector<std::vector<int>> matrixMultiplySingleThread(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();

    std::vector<std::vector<int>> C(n, std::vector<int>(p, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

int main() {
    int n = 1000; 
    int m = 1000;
    int p = 1000;

    omp_set_num_threads(omp_get_num_procs());

    // Инициализация матриц A и B
    vector<vector<int>> A(n, vector<int>(m, 1));
    vector<vector<int>> B(m, vector<int>(p, 1));

    
    auto start_time_static = std::chrono::high_resolution_clock::now();
    vector<vector<int>> C = matrixMultiplyStatic(A, B);
    auto end_time_static = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_static = end_time_static - start_time_static;


    auto start_time_dynamic = std::chrono::high_resolution_clock::now();
    vector<vector<int>> E = matrixMultiplyDynamic(A, B);
    auto end_time_dynamic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_dynamic = end_time_dynamic - start_time_dynamic;


    auto start_time_single = std::chrono::high_resolution_clock::now();
    vector<vector<int>> D = matrixMultiplySingleThread(A, B);
    auto end_time_single = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end_time_single - start_time_single;

    cout << "Time pragma omp parallel for schedule(static): " << time_static.count() << " seconds" << endl;
    cout << "Time pragma omp parallel for schedule(dynamic): " << time_dynamic.count() << " seconds" << endl;
    cout << "Time single: " << time.count() << " seconds" << endl;

    return 0;
}