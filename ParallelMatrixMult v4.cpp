#include <iostream>
#include <chrono>
#include <device_launch_parameters.h>
#include <cuda_runtime_api.h>

// Количество блоков и потоков на блок в данном примере
#define BLOCK_SIZE 16

// Функция для умножения матриц на устройстве CUDA
__global__ void multiplyMatricesCUDA(double* A, double* B, double* C, int rowsA, int colsA, int colsB) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rowsA && col < colsB) {
        double sum = 0.0;
        for (int k = 0; k < colsA; ++k) {
            sum += A[row * colsA + k] * B[k * colsB + col];
        }
        C[row * colsB + col] = sum;
    }
}

// Функция для умножения матриц на хосте (CPU)
void multiplyMatricesCPU(double* A, double* B, double* C, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i * colsB + j] = 0;
            for (int k = 0; k < colsA; k++) {
                C[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
            }
        }
    }
}

int main() {
    int row1, col1, row2, col2;
    std::cout << "row1: ";
    std::cin >> row1;
    std::cout << "col1: ";
    std::cin >> col1;
    std::cout << "row2: ";
    std::cin >> row2;
    std::cout << "col2: ";
    std::cin >> col2;

    if (col1 != row2) {
        std::cout << "Error!" << std::endl;
        return 0;
    }

    // Выделение памяти на хосте
    double* h_A = new double[row1 * col1];
    double* h_B = new double[row2 * col2];
    double* h_C_CPU = new double[row1 * col2];
    double* h_C_GPU = new double[row1 * col2];

    // Заполнение матриц случайными значениями на хосте
    for (int i = 0; i < row1 * col1; ++i) {
        h_A[i] = rand() % 100;
    }

    for (int i = 0; i < row2 * col2; ++i) {
        h_B[i] = rand() % 100;
    }

    // Выделение памяти на устройстве
    double* d_A, * d_B, * d_C;
    cudaMalloc((void**)&d_A, row1 * col1 * sizeof(double));
    cudaMalloc((void**)&d_B, row2 * col2 * sizeof(double));
    cudaMalloc((void**)&d_C, row1 * col2 * sizeof(double));

    // Копирование данных с хоста на устройство
    cudaMemcpy(d_A, h_A, row1 * col1 * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, row2 * col2 * sizeof(double), cudaMemcpyHostToDevice);

    // Вычисление размеров сетки и блока
    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocksPerGrid((col2 + threadsPerBlock.x - 1) / threadsPerBlock.x, (row1 + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Засекаем время начала выполнения на GPU
    cudaDeviceSynchronize();
    auto start_time_GPU = std::chrono::high_resolution_clock::now();

    // Вызов ядра CUDA для умножения матриц
    multiplyMatricesCUDA << <blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, row1, col1, col2);

    // Засекаем время окончания выполнения на GPU
    cudaDeviceSynchronize();
    auto end_time_GPU = std::chrono::high_resolution_clock::now();

    // Вывод времени выполнения на GPU в миллисекундах
    auto elapsed_time_GPU = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_GPU - start_time_GPU).count();
    std::cout << "GPU: " << elapsed_time_GPU << " ms" << std::endl;



    // Засекаем время начала выполнения на CPU
    auto start_time_CPU = std::chrono::high_resolution_clock::now();

    // Вызов функции умножения матриц на хосте (CPU)
    multiplyMatricesCPU(h_A, h_B, h_C_CPU, row1, col1, col2);

    // Засекаем время окончания выполнения на CPU
    auto end_time_CPU = std::chrono::high_resolution_clock::now();
    auto elapsed_time_CPU = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_CPU - start_time_CPU).count();

    // Вывод времени выполнения на CPU в миллисекундах
    std::cout << "CPU: " << elapsed_time_CPU << " ms" << std::endl;

    // Копирование результата с устройства на хост
    cudaMemcpy(h_C_GPU, d_C, row1 * col2 * sizeof(double), cudaMemcpyDeviceToHost);

    // Освобождение памяти на устройстве и хосте
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    delete[] h_A;
    delete[] h_B;
    delete[] h_C_CPU;
    delete[] h_C_GPU;

    return 0;
}

