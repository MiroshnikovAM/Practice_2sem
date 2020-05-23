#include "practice.h"

using namespace std;

void FillData(INPUT_DATA & data) {
    // Do not delete it's for debug
    /*
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
    */
    cout << "Input k = ";
    cin >> data.k;
    cout << "Input l = ";
    cin >> data.l;
    cout << "Input M[" << data.k << "] : " << endl;
    data.M = new size_t [data.k];
    for (size_t i = 0; i < data.k; i++) {
        cin >> data.M[i];
    }
    cout << "Input T as time in milleseconds (2000 for 2 sec) = ";
    cin >> data.T;
    cout << "Input N = ";
    cin >> data.N;
    cout << "Input A[" << data.l << "][" << data.k + 1 << "] : " << endl;
    data.A = new double* [data.l];
    for (size_t i = 0; i < data.l; i++) {
        data.A[i] = new double [data.k+1];
        for (size_t j = 0; j < data.k+1; j++) {
            cin >> data.A[i][j];
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

void WriteCalculatedVector(const INPUT_DATA & data, char* filename, bool shouldAdd, double* z) {
    ofstream file;
    if (shouldAdd) {
        file.open(filename, ios::binary | ios::out | ios::app);
    } else {
        file.open(filename, ios::binary | ios::out | ios::trunc);
    }
    for (size_t i = 0; i < data.l; i++) {
        cout << z[i] << "\t";
        file << z[i] << endl;
    }
    cout << endl;
    file.close();
}

void ReadCalculatedVectors(const INPUT_DATA & data, char* filename, double*** pZZ) {
    ifstream file;
    file.open(filename, ios::binary | ios::in);
    *pZZ = new double* [data.N];
    for (size_t i = 0; i < data.N; i++) {
        (*pZZ)[i] = new double [data.l];
        for (size_t j = 0; j < data.l; j++) {
            file >> (*pZZ)[i][j];
        }
    }
    file.close();
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

