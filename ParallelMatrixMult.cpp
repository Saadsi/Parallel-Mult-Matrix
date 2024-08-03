#include <iostream>
#include <chrono>
#include <Windows.h>

using namespace std;
using namespace chrono;

// ������� ��� ��������� ���� ������
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
    cout << "�������� ����� (1 - ��������, 2 - ���������): ";
    cin >> mode;

    if (mode == 1) {
        int row1, col1, row2, col2;
        cout << "������� ���������� ����� ������ �������: ";
        cin >> row1;
        cout << "������� ���������� �������� ������ �������: ";
        cin >> col1;
        cout << "������� ���������� ����� ������ �������: ";
        cin >> row2;
        cout << "������� ���������� �������� ������ �������: ";
        cin >> col2;

        if (col1 != row2) {
            cout << "��������� ����������!" << endl;
            cin.get(); cin.get();
            return 0;
        }

        // ���� ��������� ������ �������
        double** a = new double* [row1];
        cout << "������� �������� ������ �������" << endl;
        for (int i = 0; i < row1; i++) {
            a[i] = new double[col1];
            for (int j = 0; j < col1; j++) {
                cout << "a[" << i << "][" << j << "]= ";
                cin >> a[i][j];
            }
        }

        // ���� ��������� ������ �������
        double** b = new double* [row2];
        cout << "������� �������� ������ �������" << endl;
        for (int i = 0; i < row2; i++) {
            b[i] = new double[col2];
            for (int j = 0; j < col2; j++) {
                cout << "b[" << i << "][" << j << "]= ";
                cin >> b[i][j];
            }
        }

        // ��������� ������
        double** c = new double* [row1];
        for (int i = 0; i < row1; i++) {
            c[i] = new double[col2];
        }

        multiplyMatrices(a, b, c, row1, col1, col2);

        // ����� ������� ������������
        cout << "������� ������������:" << endl;
        for (int i = 0; i < row1; i++) {
            for (int j = 0; j < col2; j++) {
                cout << c[i][j] << " ";
            }
            cout << endl;
        }

        // ������������ ������
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
        // ��������� ����� � ���������� ������

        int row1, col1, row2, col2;
        cout << "������� ���������� ����� ������ �������: ";
        cin >> row1;
        cout << "������� ���������� �������� ������ �������: ";
        cin >> col1;
        cout << "������� ���������� ����� ������ �������: ";
        cin >> row2;
        cout << "������� ���������� �������� ������ �������: ";
        cin >> col2;

        if (col1 != row2) {
            cout << "��������� ����������!" << endl;
            return 0;
        }

        // ��������� ������
        double** a = new double* [row1];
        double** b = new double* [row2];
        for (int i = 0; i < row1; i++) {
            a[i] = new double[col1];
        }

        for (int i = 0; i < row2; i++) {
            b[i] = new double[col2];
        }

        // ���������� ������ ���������� ����������
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

        // �������� ����� ������ ����������
        high_resolution_clock::time_point start_time = high_resolution_clock::now();

        // ��������� ������
        double** c = new double* [row1];
        for (int i = 0; i < row1; i++) {
            c[i] = new double[col2];
        }

        multiplyMatrices(a, b, c, row1, col1, col2);

        // �������� ����� ��������� ����������
        high_resolution_clock::time_point end_time = high_resolution_clock::now();
        duration<double, milli> elapsed_time = end_time - start_time;

        // ����� ������� ���������� � �������������
        cout << "����� ���������� ��������� ������ � ��������� ������: " << elapsed_time.count() << " �����������" << endl;

        // ������������ ������
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
        cout << "�������� �����." << endl;
    }

    return 0;
}
