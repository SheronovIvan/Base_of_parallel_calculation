#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

using namespace std;

// size
const int N = 40; 
const int M = 150; 

void initializeField(vector<vector<int>>& field) {
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            field[i][j] = rand() % 2; // death 0 or 1 live
        }
    }
}


int countLiveNeighbors(const vector<vector<int>>& field, int x, int y) {
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; 
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < N && ny >= 0 && ny < M) {
                liveNeighbors += field[nx][ny];
            }
        }
    }
    return liveNeighbors;
}


void updateField(const vector<vector<int>>& current, vector<vector<int>>& next) {
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int liveNeighbors = countLiveNeighbors(current, i, j);
            if (current[i][j] == 1) {
                // rule for live
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    next[i][j] = 0; // death by lonely or pere lives
                }
                else {
                    next[i][j] = 1; // Выживает
                }
            }
            else {
                // rule for death
                if (liveNeighbors == 3) {
                    next[i][j] = 1; // born
                }
                else {
                    next[i][j] = 0; // stay death
                }
            }
        }
    }
}

void printField(const vector<vector<int>>& field) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << (field[i][j] ? "#" : "."); // live: #, death: .
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    vector<vector<int>> currentField(N, vector<int>(M)); 
    vector<vector<int>> nextField(N, vector<int>(M));    

    
    initializeField(currentField);

    int iterations = 50;
    for (int iter = 0; iter < iterations; ++iter) {
       
        updateField(currentField, nextField);

       
        currentField = nextField;

        
        cout << "Iteration: " << iter + 1 << endl;
        printField(currentField);

        
        int liveCells = 0;
#pragma omp parallel for reduction(+:liveCells)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                liveCells += currentField[i][j];
            }
        }
        cout << "Live cells: " << liveCells << endl << endl;

        
        system("timeout 1 >nul"); 
    }

    return 0;
}