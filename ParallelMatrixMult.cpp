#include <iostream>
#include <chrono>
#include <Windows.h>

using namespace std;
using namespace chrono;

// Функция для умножения двух матриц
void multiplyMatrices(double** A, double** B, double** C, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int mode;
    cout << "Выберите режим (1 - тестовый, 2 - расчетный): ";
    cin >> mode;

    if (mode == 1) {
        int row1, col1, row2, col2;
        cout << "Введите количество строк первой матрицы: ";
        cin >> row1;
        cout << "Введите количество столбцов первой матрицы: ";
        cin >> col1;
        cout << "Введите количество строк второй матрицы: ";
        cin >> row2;
        cout << "Введите количество столбцов второй матрицы: ";
        cin >> col2;

        if (col1 != row2) {
            cout << "Умножение невозможно!" << endl;
            cin.get(); cin.get();
            return 0;
        }

        // Ввод элементов первой матрицы
        double** a = new double* [row1];
        cout << "Введите элементы первой матрицы" << endl;
        for (int i = 0; i < row1; i++) {
            a[i] = new double[col1];
            for (int j = 0; j < col1; j++) {
                cout << "a[" << i << "][" << j << "]= ";
                cin >> a[i][j];
            }
        }

        // Ввод элементов второй матрицы
        double** b = new double* [row2];
        cout << "Введите элементы второй матрицы" << endl;
        for (int i = 0; i < row2; i++) {
            b[i] = new double[col2];
            for (int j = 0; j < col2; j++) {
                cout << "b[" << i << "][" << j << "]= ";
                cin >> b[i][j];
            }
        }

        // Умножение матриц
        double** c = new double* [row1];
        for (int i = 0; i < row1; i++) {
            c[i] = new double[col2];
        }

        multiplyMatrices(a, b, c, row1, col1, col2);

        // Вывод матрицы произведения
        cout << "Матрица произведения:" << endl;
        for (int i = 0; i < row1; i++) {
            for (int j = 0; j < col2; j++) {
                cout << c[i][j] << " ";
            }
            cout << endl;
        }

        // Освобождение памяти
        for (int i = 0; i < row1; i++) {
            delete[] c[i];
        }
        delete[] c;

        for (int i = 0; i < row1; i++) {
            delete[] a[i];
        }
        delete[] a;

        for (int i = 0; i < row2; i++) {
            delete[] b[i];
        }
        delete[] b;
    }
    else if (mode == 2) {
        // Расчетный режим с генерацией матриц

        int row1, col1, row2, col2;
        cout << "Введите количество строк первой матрицы: ";
        cin >> row1;
        cout << "Введите количество столбцов первой матрицы: ";
        cin >> col1;
        cout << "Введите количество строк второй матрицы: ";
        cin >> row2;
        cout << "Введите количество столбцов второй матрицы: ";
        cin >> col2;

        if (col1 != row2) {
            cout << "Умножение невозможно!" << endl;
            return 0;
        }

        // Генерация матриц
        double** a = new double* [row1];
        double** b = new double* [row2];
        for (int i = 0; i < row1; i++) {
            a[i] = new double[col1];
        }

        for (int i = 0; i < row2; i++) {
            b[i] = new double[col2];
        }

        // Заполнение матриц случайными значениями
        for (int i = 0; i < row1; i++) {
            for (int j = 0; j < col1; j++) {
                a[i][j] = rand() % 100;
            }
        }

        for (int i = 0; i < row2; i++) {
            for (int j = 0; j < col2; j++) {
                b[i][j] = rand() % 100;
            }
        }

        // Засекаем время начала выполнения
        high_resolution_clock::time_point start_time = high_resolution_clock::now();

        // Умножение матриц
        double** c = new double* [row1];
        for (int i = 0; i < row1; i++) {
            c[i] = new double[col2];
        }

        multiplyMatrices(a, b, c, row1, col1, col2);

        // Засекаем время окончания выполнения
        high_resolution_clock::time_point end_time = high_resolution_clock::now();
        duration<double, milli> elapsed_time = end_time - start_time;

        // Вывод времени выполнения в миллисекундах
        cout << "Время выполнения умножения матриц в расчетном режиме: " << elapsed_time.count() << " миллисекунд" << endl;

        // Освобождение памяти
        for (int i = 0; i < row1; i++) {
            delete[] a[i];
            delete[] c[i];
        }
        delete[] a;
        delete[] c;

        for (int i = 0; i < row2; i++) {
            delete[] b[i];
        }
        delete[] b;
    }
    else {
        cout << "Неверный режим." << endl;
    }

    return 0;
}
