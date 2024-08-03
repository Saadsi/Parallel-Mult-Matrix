#include <iostream>
#include <chrono>
#include <cstdlib>
#include <mpi.h>

using namespace std;
using namespace chrono;

// Функция для умножения части матрицы на другую матрицу
void multiplyMatrices(double* A, double* B, double* C, int rowsA, int colsA, int colsB, int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i * colsB + j] = 0;
            for (int k = 0; k < colsA; k++) {
                C[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int mode = 0;
    if (rank == 0) {
        cout << "Выберите режим (1 - тестовый, 2 - расчетный): ";
        cin >> mode;
    }

    MPI_Bcast(&mode, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int numThreads = 0; // Количество ядер для MPI

    if (mode == 2) {
        if (rank == 0) {
            cout << "Введите количество ядер (потоков) для MPI: ";
            cin >> numThreads;
        }

        MPI_Bcast(&numThreads, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (mode == 1) {
        // Режим тестирования
        // ...

    }
    else if (mode == 2) {
        // Расчетный режим с генерацией матриц
        int row1, col1, row2, col2;

        if (rank == 0) {
            cout << "Введите количество строк первой матрицы: ";
            cin >> row1;
            cout << "Введите количество столбцов первой матрицы: ";
            cin >> col1;
            cout << "Введите количество строк второй матрицы: ";
            cin >> row2;
            cout << "Введите количество столбцов второй матрицы: ";
            cin >> col2;
        }

        MPI_Bcast(&row1, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&col1, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&row2, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&col2, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (col1 != row2) {
            if (rank == 0) {
                cout << "Умножение невозможно!" << endl;
            }
        }
        else {
            // Генерация матриц
            if (rank == 0) {
                srand(time(NULL));
            }

            int chunkSize = row1 / size; // Количество строк на каждый процесс

            // Распределение данных
            double* a = new double[row1 * col1];
            double* b = new double[row2 * col2];

            if (rank == 0) {
                for (int i = 0; i < row1 * col1; i++) {
                    a[i] = rand() % 100;
                }

                for (int i = 0; i < row2 * col2; i++) {
                    b[i] = rand() % 100;
                }
            }

            MPI_Bcast(a, row1 * col1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Bcast(b, row2 * col2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // Выделение памяти для результатов
            double* c = new double[row1 * col2];

            // Засекаем время начала выполнения
            high_resolution_clock::time_point start_time = high_resolution_clock::now();

            // Умножение матриц
            multiplyMatrices(a, b, c, row1, col1, col2, rank * chunkSize, (rank + 1) * chunkSize);

            // Сбор результатов
            MPI_Gather(c + rank * chunkSize * col2, chunkSize * col2, MPI_DOUBLE, c, chunkSize * col2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // Засекаем время окончания выполнения
            high_resolution_clock::time_point end_time = high_resolution_clock::now();
            duration<double, milli> elapsed_time = end_time - start_time;

            // Вывод времени выполнения в миллисекундах
            if (rank == 0) {
                cout << "Время выполнения умножения матриц в расчетном режиме: " << elapsed_time.count() << " миллисекунд" << endl;
            }

            // Освобождение памяти
            delete[] a;
            delete[] b;
            delete[] c;
        }
    }
    else {
        if (rank == 0) {
            cout << "Неверный режим." << endl;
        }
    }

    MPI_Finalize();

    return 0;
}
