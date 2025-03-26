#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000000;  // 10^6 
    std::vector<int> data;
    int localSum = 0, globalSum = 0, globalSumAsync = 0;

    
    if (rank == 0) {
        data.resize(N);
        srand(42);  
        for (int i = 0; i < N; ++i) {
            data[i] = rand() % 100;  //0-99
        }
    }

    
    int localSize = N / size;
    std::vector<int> localData(localSize);

    //seq
    double sequentialTime = 0;
    int sequentialSum = 0;

    if (rank == 0) {
        auto startSeq = std::chrono::high_resolution_clock::now();
        for (int val : data) sequentialSum += val;
        auto endSeq = std::chrono::high_resolution_clock::now();
        sequentialTime = std::chrono::duration<double>(endSeq - startSeq).count();
    }


    //MPI_Reduce
    auto start = std::chrono::high_resolution_clock::now();

    MPI_Scatter(data.data(), localSize, MPI_INT, localData.data(), localSize, MPI_INT, 0, MPI_COMM_WORLD);

    for (int value : localData)
        localSum += value;
  
    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    auto end = std::chrono::high_resolution_clock::now();
    double reduceTime = std::chrono::duration<double>(end - start).count();


    if (rank == 0) {
        std::cout << "Sequential:  " << sequentialSum << " (Time: " << sequentialTime << " sec)\n";
        std::cout << "MPI_Reduce:  " << globalSum << " (Time: " << reduceTime << " sec)\n";
    }

    MPI_Finalize();
    return 0;
}
