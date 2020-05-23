#include <iostream>
#include <vector>
#include "plant.h"
#include <windows.h>
#include <fstream>

using namespace std;

struct INPUT_DATA {
    size_t k;
    size_t* M;
    size_t T;
    size_t N;
    size_t l;
    double** A;
};

void FillData(INPUT_DATA & data);
void DisposeData(INPUT_DATA & data);
void DisposeVector(double** ptrVector);
void DisposeMatrix(const INPUT_DATA & data, double*** ptrMatrix);
void GetCurrentVector(const INPUT_DATA & data, Plant plant, double** ptrVector);
void GetCalculatedVector(const INPUT_DATA & data, double* x, double** pZ);
void WriteCalculatedVector(const INPUT_DATA & data, ofstream file, double* z);
void ReadCalculatedVectors(const INPUT_DATA & data, ifstream file, double*** pZZ);
double CalculateAverage(const INPUT_DATA & data, double* vector);
double CalculateVariance(const INPUT_DATA & data, double average, double* vector);


int main() {
    INPUT_DATA params;
    FillData(params);
    Plant plant;
    plant_init(plant);

    fstream file;
    file.open("data.raw", ios::binary | ios::out | ios::trunc | ios::in);

    double* x = 0;
    double* z = 0;

    for (size_t i = 0; i < params.N; i++) {
        GetCurrentVector(params, plant, &x);
        GetCalculatedVector(params, x, &z);
        WriteCalculatedVector(params, (ofstream)file, z);
        Sleep(params.T);
    }
    DisposeVector(&x);
    DisposeVector(&z);

    double** zz = 0;

    ReadCalculatedVectors(params, (ifstream)file, &zz);
    double average;
    double variance;
    for (size_t i = 0; i < params.l; i++) {
        average = CalculateAverage(params, zz[i]);
        variance = CalculateVariance(params, average, zz[i]);
        cout << "avg[" << i << "] = " << average << endl;
        cout << "var[" << i << "] = " << variance << endl;
    }
    DisposeMatrix(params, &zz);

    DisposeData(params);
    return 0;
}


void FillData(INPUT_DATA & data) {
    data.k = 4;
    data.M = new size_t [data.k];
    for (size_t i = 0; i < data.k; i++) {
        data.M[i] = i + 1;
    }
    data.T = 2000;
    data.N = 5;
    data.l = 2;
    data.A = new double* [data.l];
    for (size_t i = 0; i < data.l; i++) {
        data.A[i] = new double [data.k+1];
        for (size_t j = 0; j < data.k+1; j++) {
            data.A[i][j] = 1;
        }
    }
}

void DisposeData(INPUT_DATA & data) {
    if (data.M) {
        delete data.M;
        data.M = 0;
    }
    if (data.A) {
        for (size_t i = 0; i < data.l; i++) {
            if (data.A[i]) {
                delete data.A[i];
                data.A[i] = 0;
            }
        }
        delete data.A;
        data.A = 0;
    }
}

void DisposeVector(double** ptrVector) {
    if (*ptrVector) {
        delete *ptrVector;
        *ptrVector = 0;
    }
}

void DisposeMatrix(const INPUT_DATA & data, double*** ptrMatrix) {
    if (*ptrMatrix) {
        for (size_t i = 0; i < data.N; i++) {
            if ((*ptrMatrix)[i]) {
                delete (*ptrMatrix)[i];
                (*ptrMatrix)[i] = 0;
            }
        }
        *ptrMatrix = 0;
    }
}

void GetCurrentVector(const INPUT_DATA & data, Plant plant, double** ptrVector) {
    if (*ptrVector == 0) {
        *ptrVector = new double [data.k];
    }
    for (size_t i = 0; i < data.k; i++) {
        (*ptrVector)[i] = plant_measure(data.M[i], plant);
    }
}

void GetCalculatedVector(const INPUT_DATA & data, double* x, double** pZ) {
    if (*pZ == 0) {
        *pZ = new double [data.l];
    }
    for (size_t i = 0; i < data.l; i++) {
        (*pZ)[i] = data.A[i][0];
        for (size_t j = 0; j < data.k; j++) {
            (*pZ)[i] += data.A[i][j+1] * x[j];
        }
    }
}

void WriteCalculatedVector(const INPUT_DATA & data, ofstream file, double* z) {
    for (size_t i = 0; i < data.l; i++) {
        cout << z[i] << "\t";
        file << z[i] << endl;
    }
    cout << endl;
}

void ReadCalculatedVectors(const INPUT_DATA & data, ifstream file, double*** pZZ) {
    *pZZ = new double* [data.N];
    for (size_t i = 0; i < data.N; i++) {
        (*pZZ)[i] = new double [data.l];
        /*
        for (size_t j = 0; j < data.l; j++) {
            (*pZZ)[i][j] = 1; // ToDo
        }
        */
        for (size_t j = 0; j < data.l; j++) {
            file >> (*pZZ)[i][j];
        }
    }
}

double CalculateAverage(const INPUT_DATA & data, double* vector) {
    double sum = 0;
    for (size_t i = 0; i < data.N; i++) {
        sum += vector[i];
    }
    return data.N ? sum/data.N : 0;
}

double CalculateVariance(const INPUT_DATA & data, double average, double* vector) {
    double square_sum = 0;
    for (size_t i = 0; i < data.N; i++) {
        square_sum += vector[i] * vector[i];
    }
    if (data.N <= 1) {
        return 0;
    } else {
        return (square_sum - data.N * average * average) / (data.N - 1);
    }
}
