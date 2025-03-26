#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

#define N 500  

void print_matrix(const std::vector<int>& matrix, int rows, int cols, int max_print = 5) {
    for (int i = 0; i < std::min(rows, max_print); i++) {
        for (int j = 0; j < std::min(cols, max_print); j++) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << "...\n";
    }
    std::cout << "...\n";
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_rows = N / size; 

    
    std::vector<int> A, B(N * N), C;
    std::vector<int> local_A(local_rows * N), local_C(local_rows * N, 0);

    if (rank == 0) {
        A.resize(N * N);
        C.resize(N * N);

        
        srand(time(nullptr));
        for (int i = 0; i < N * N; i++) {
            A[i] = rand() % 10 + 1;
            B[i] = rand() % 10 + 1;
        }

        //std::cout << "Matrix A (part):\n";
        //print_matrix(A, N, N);
        //std::cout << "Matrix B (part):\n";
        //print_matrix(B, N, N);
    }

    
    auto start_time = std::chrono::high_resolution_clock::now();

    
    MPI_Scatter(A.data(), local_rows * N, MPI_INT, local_A.data(), local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    
    MPI_Bcast(B.data(), N * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }

    MPI_Gather(local_C.data(), local_rows * N, MPI_INT, C.data(), local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();

    if (rank == 0) {
        std::cout << "Result matrix C (part):\n";
        print_matrix(C, N, N);
        std::cout << "Execution time: " << elapsed_time << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
