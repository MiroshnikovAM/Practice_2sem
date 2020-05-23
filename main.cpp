#include "practice.h"

using namespace std;

void PrintCurrentVector(const INPUT_DATA & data, size_t step, double* vector);
void PrintHeader(const INPUT_DATA & data);
void PrintFooter(const INPUT_DATA & data, double average, double variance, size_t step);

int main() {
    INPUT_DATA params;
    FillData(params);
    Plant plant;
    plant_init(plant);

    char* filename = "data.raw";

    double* x = 0;
    double* z = 0;
    PrintHeader(params);
    for (size_t i = 0; i < params.N; i++) {
        GetCurrentVector(params, plant, &x);
        GetCalculatedVector(params, x, &z);
        PrintCurrentVector(params, i, x);
        WriteCalculatedVector(params, filename, (i != 0), z);
        Sleep(params.T);
    }
    DisposeVector(&x);
    DisposeVector(&z);

    double** zz = 0;

    ReadCalculatedVectors(params, filename, &zz);
    double average;
    double variance;
    for (size_t i = 0; i < params.l; i++) {
        average = CalculateAverage(params, zz[i]);
        variance = CalculateVariance(params, average, zz[i]);
        PrintFooter(params, average, variance, i);
    }
    DisposeMatrix(params, &zz);
    DisposeData(params);
    return 0;
}

void PrintCurrentVector(const INPUT_DATA & data, size_t step, double* vector) {
    cout << step + 1 << "\t";
    for (size_t i = 0; i < data.k; i++) {
        cout << vector[i] << "\t";
    }
}

void PrintHeader(const INPUT_DATA & data) {
    cout << "N\t";
    for (size_t i = 0; i < data.k; i++) {
        cout << "X" << i + 1 << "\t";
    }
    for (size_t i = 0; i < data.l; i++) {
        cout << "Z" << i + 1 << "\t";
    }
    cout << endl;
}

void PrintFooter(const INPUT_DATA & data, double average, double variance, size_t step) {
    for (size_t j = 0; j < data.k; j++) {
        cout << "\t";
    }
    cout << "Zavg[" << step + 1 << "]";
    for (size_t j = 0; j < step + 1; j++) {
        cout << "\t";
    }
    cout << average << endl;
    for (size_t j = 0; j < data.k; j++) {
        cout << "\t";
    }
    cout << "S^2 [" << step + 1 << "]";
    for (size_t j = 0; j < step + 1; j++) {
        cout << "\t";
    }
    cout << variance << endl;
}
