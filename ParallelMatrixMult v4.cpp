#include <iostream>
#include <chrono>
#include <device_launch_parameters.h>
#include <cuda_runtime_api.h>

// ���������� ������ � ������� �� ���� � ������ �������
#define BLOCK_SIZE 16

// ������� ��� ��������� ������ �� ���������� CUDA
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

// ������� ��� ��������� ������ �� ����� (CPU)
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

    // ��������� ������ �� �����
    double* h_A = new double[row1 * col1];
    double* h_B = new double[row2 * col2];
    double* h_C_CPU = new double[row1 * col2];
    double* h_C_GPU = new double[row1 * col2];

    // ���������� ������ ���������� ���������� �� �����
    for (int i = 0; i < row1 * col1; ++i) {
        h_A[i] = rand() % 100;
    }

    for (int i = 0; i < row2 * col2; ++i) {
        h_B[i] = rand() % 100;
    }

    // ��������� ������ �� ����������
    double* d_A, * d_B, * d_C;
    cudaMalloc((void**)&d_A, row1 * col1 * sizeof(double));
    cudaMalloc((void**)&d_B, row2 * col2 * sizeof(double));
    cudaMalloc((void**)&d_C, row1 * col2 * sizeof(double));

    // ����������� ������ � ����� �� ����������
    cudaMemcpy(d_A, h_A, row1 * col1 * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, row2 * col2 * sizeof(double), cudaMemcpyHostToDevice);

    // ���������� �������� ����� � �����
    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocksPerGrid((col2 + threadsPerBlock.x - 1) / threadsPerBlock.x, (row1 + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // �������� ����� ������ ���������� �� GPU
    cudaDeviceSynchronize();
    auto start_time_GPU = std::chrono::high_resolution_clock::now();

    // ����� ���� CUDA ��� ��������� ������
    multiplyMatricesCUDA << <blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, row1, col1, col2);

    // �������� ����� ��������� ���������� �� GPU
    cudaDeviceSynchronize();
    auto end_time_GPU = std::chrono::high_resolution_clock::now();

    // ����� ������� ���������� �� GPU � �������������
    auto elapsed_time_GPU = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_GPU - start_time_GPU).count();
    std::cout << "GPU: " << elapsed_time_GPU << " ms" << std::endl;



    // �������� ����� ������ ���������� �� CPU
    auto start_time_CPU = std::chrono::high_resolution_clock::now();

    // ����� ������� ��������� ������ �� ����� (CPU)
    multiplyMatricesCPU(h_A, h_B, h_C_CPU, row1, col1, col2);

    // �������� ����� ��������� ���������� �� CPU
    auto end_time_CPU = std::chrono::high_resolution_clock::now();
    auto elapsed_time_CPU = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_CPU - start_time_CPU).count();

    // ����� ������� ���������� �� CPU � �������������
    std::cout << "CPU: " << elapsed_time_CPU << " ms" << std::endl;

    // ����������� ���������� � ���������� �� ����
    cudaMemcpy(h_C_GPU, d_C, row1 * col2 * sizeof(double), cudaMemcpyDeviceToHost);

    // ������������ ������ �� ���������� � �����
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    delete[] h_A;
    delete[] h_B;
    delete[] h_C_CPU;
    delete[] h_C_GPU;

    return 0;
}

