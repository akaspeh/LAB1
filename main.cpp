#include <iostream>
#include <thread>
#include <cstdlib>
#include "tracy/Tracy.hpp"


using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

//Заповнити квадратну матрицю випадковими числами. Відобразити
//матрицю симетрично відносно головної діагоналі.


int** createMatrix(int rows, int cols){
    int** arr = new int*[rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
    }
    return arr;
}

bool fillMatrix(int** arr, int rows, int cols, int threadsAmount, int thread_id){
    ZoneScopedN("fillMatrix");
    int i = thread_id / cols;
    for(; i < rows; i++){
        int j = thread_id % cols;
        for(; j < cols; j = j+threadsAmount){
            arr[i][j] = std::rand() % 9 + 1;
        }
    }
    return true;
}

void printMatrix(int **arr, int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            std::cout << arr[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void sideSwap(int** arr, int** newArr, int rows, int cols, int threadsAmount, int thread_id){
    ZoneScopedN("sideSwap");
    int i = thread_id / cols;// індекс рядка
    for(; i < rows; i++){
        int j = thread_id % cols; // індекс колонки
        for(; j < cols; j = j+threadsAmount){ // те саме зрушення
//            std::cout << "thread num "<<thread_id<< "\n";
            newArr[j][i] = arr[rows - 1 - i][cols - 1 - j]; // переписування в іншу матрицю
        }
    }
}


int main() {
    FrameMark;
    srand(time(NULL));
    int threadsAmount = 64;
    int rows = 50;
    int cols = 50;
    std::thread* threads = new std::thread[threadsAmount];

    int **matrix = createMatrix(rows,cols);
    int **matrixSymmetric = createMatrix(rows,cols);

    for (int i = 0; i < threadsAmount; i++) {
        threads[i] = std::thread(fillMatrix, matrix, rows, cols, threadsAmount, i);
    }

    auto payload_begin1 = high_resolution_clock::now();
    for (int i = 0; i < threadsAmount; i++) {
        threads[i].join();
    }
    auto payload_end1 = high_resolution_clock::now();
    auto elapsed1 = duration_cast<nanoseconds>(payload_end1 - payload_begin1);
    std::printf("fillMatrix Payload Time: %.4f seconds.\n", elapsed1.count() * 1e-9);
    //printMatrix(matrix, rows, cols);

    for (int i = 0; i < threadsAmount; i++) {
        threads[i] = std::thread(sideSwap, matrix, matrixSymmetric, rows, cols, threadsAmount, i);
    }

    auto payload_begin2 = high_resolution_clock::now();
    for (int i = 0; i < threadsAmount; i++) {
        threads[i].join();
    }
    auto payload_end2 = high_resolution_clock::now();
    auto elapsed2 = duration_cast<nanoseconds>(payload_end2 - payload_begin2);
    std::printf("swapSide Payload Time: %.4f seconds.\n", elapsed2.count() * 1e-9);

    std::cout << "\n";
    //printMatrix(matrixSymmetric, rows, cols);


    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] matrixSymmetric;
    delete[] threads;
    return 0;
}
