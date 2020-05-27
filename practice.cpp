#include "practice.h"

using namespace std;

void GetParams(PARAMS & params) {
    /*
    // Do not delete it's for debug
    params.k = 4;
    params.M = new size_t [params.k];
    // for (size_t i = 0; i < params.k; i++) {
    //     params.M[i] = i + 1;
    // }
    params.M[0] = 3;
    params.M[1] = 2;
    params.M[2] = 4;
    params.M[3] = 1;
    params.T = 2000;
    params.N = 5;
    params.l = 2;
    params.A = new double* [params.l];
    for (size_t i = 0; i < params.l; i++) {
        params.A[i] = new double [params.k+1];
        // for (size_t j = 0; j < params.k+1; j++) {
        //     params.A[i][j] = 1;
        // }
    }
    params.A[0][0] = 0;
    params.A[0][1] = 0.1;
    params.A[0][2] = 0.01;
    params.A[0][3] = 12;
    params.A[0][4] = 0.5;
    params.A[1][0] = 0;
    params.A[1][1] = -0.1;
    params.A[1][2] = -0.01;
    params.A[1][3] = 9;
    params.A[1][4] = 0.45;
    */

    cout << "Input k = ";
    cin >> params.k;
    cout << "Input l = ";
    cin >> params.l;
    cout << "Input M[" << params.k << "] : " << endl;
    params.M = new size_t [params.k];
    for (size_t i = 0; i < params.k; i++) {
        cin >> params.M[i];
    }
    cout << "Input T as time in milleseconds (2000 for 2 sec) = ";
    cin >> params.T;
    cout << "Input N = ";
    cin >> params.N;
    cout << "Input A[" << params.l << "][" << params.k + 1 << "] : " << endl;
    params.A = new double* [params.l];
    for (size_t i = 0; i < params.l; i++) {
        params.A[i] = new double [params.k+1];
        for (size_t j = 0; j < params.k+1; j++) {
            cin >> params.A[i][j];
        }
    }

}

void GetTestParams(PARAMS & params) {
    /*
    // Do not delete it's for debug
    params.k = 4;
    params.M = new size_t [params.k];
    params.M[0] = 3;
    params.M[1] = 2;
    params.M[2] = 4;
    params.M[3] = 1;
    params.T = 2000;
    params.N = 5;
    params.l = 1;
    params.A = new double* [params.l];
    for (size_t i = 0; i < params.l; i++) {
        params.A[i] = new double [params.k+1];
    }
    params.A[0][0] = 0;
    params.A[0][1] = 0.1;
    params.A[0][2] = 0.01;
    params.A[0][3] = 12;
    params.A[0][4] = 0.5;
    params.Aimin = -0.1;
    params.Aimax = 0.1;
    params.dAi = 0.01;
    params.researchCoeff = 2;
    */
    cout << "Input k = ";
    cin >> params.k;
    params.l = 1;
    cout << "Input M[" << params.k << "] : " << endl;
    params.M = new size_t [params.k];
    for (size_t i = 0; i < params.k; i++) {
        cin >> params.M[i];
    }
    cout << "Input T as time in milleseconds (2000 for 2 sec) = ";
    cin >> params.T;
    cout << "Input N = ";
    cin >> params.N;
    cout << "Input i, number of researched coefficient = ";
    cin >> params.researchCoeff;
    cout << "Input A[" << params.l << "][" << params.k + 1 << "] : " << endl;
    params.A = new double* [params.l];
    for (size_t i = 0; i < params.l; i++) {
        params.A[i] = new double [params.k+1];
        for (size_t j = 0; j < params.k+1; j++) {
            cin >> params.A[i][j];
        }
    }
    cout << "Input Aimin = ";
    cin >> params.Aimin;
    cout << "Input Aimax = ";
    cin >> params.Aimax;
    cout << "Input dAi = ";
    cin >> params.dAi;

}

void DisposeParams(PARAMS & params) {
    if (params.M) {
        delete params.M;
        params.M = 0;
    }
    if (params.A) {
        for (size_t i = 0; i < params.l; i++) {
            if (params.A[i]) {
                delete params.A[i];
                params.A[i] = 0;
            }
        }
        delete params.A;
        params.A = 0;
    }
}

void DisposeVector(double** ptrVector) {
    if (*ptrVector) {
        delete *ptrVector;
        *ptrVector = 0;
    }
}

void DisposeMatrix(const PARAMS & params, double*** ptrMatrix) {
    if (*ptrMatrix) {
        for (size_t i = 0; i < params.l; i++) {
            if ((*ptrMatrix)[i]) {
                delete (*ptrMatrix)[i];
                (*ptrMatrix)[i] = 0;
            }
        }
        delete *ptrMatrix;
        *ptrMatrix = 0;
    }
}

void GetCurrentVector(const PARAMS & params, Plant plant, double** ptrVector) {
    if (*ptrVector == 0) {
        *ptrVector = new double [params.k];
    }
    for (size_t i = 0; i < params.k; i++) {
        (*ptrVector)[i] = plant_measure(params.M[i], plant);
    }
}

void GetCalculatedVector(const PARAMS & params, double* x, double** pZ) {
    if (*pZ == 0) {
        *pZ = new double [params.l];
    }
    for (size_t i = 0; i < params.l; i++) {
        (*pZ)[i] = params.A[i][0];
        for (size_t j = 0; j < params.k; j++) {
            (*pZ)[i] += params.A[i][j+1] * x[j];
        }
    }
}

void WriteCalculatedVector(const PARAMS & params, char* filename, size_t step, double* z) {
    ofstream file;
    if (step) {
        file.open(filename, ios::binary | ios::out | ios::app);
    } else {
        file.open(filename, ios::binary | ios::out | ios::trunc);
    }
    for (size_t i = 0; i < params.l; i++) {
        // cout << z[i] << "\t";
        file << z[i] << endl;
    }
    // cout << endl;
    file.close();
}

void ReadCalculatedVectors(const PARAMS & params, char* filename, double*** pZZ) {
    *pZZ = new double* [params.l];
    for (size_t i = 0; i < params.l; i++) {
        (*pZZ)[i] = new double [params.N];
    }
    ifstream file;
    file.open(filename, ios::binary | ios::in);
    for(size_t i = 0; i < params.N; i++) {
        for (size_t j = 0; j < params.l; j++) {
            file >> (*pZZ)[j][i];
        }
    }
    file.close();
}

double CalculateAverage(const PARAMS & params, double* vector) {
    double sum = 0;
    for (size_t i = 0; i < params.N; i++) {
        sum += vector[i];
    }
    return params.N ? sum/params.N : 0;
}

double CalculateVariance(const PARAMS & params, double average, double* vector) {
    double square_sum = 0;
    for (size_t i = 0; i < params.N; i++) {
        square_sum += vector[i] * vector[i];
    }
    if (params.N <= 1) {
        return 0;
    } else {
        return (square_sum - params.N * average * average) / (params.N - 1);
    }
}

void PrintCurrentVector(const PARAMS & params, size_t step, double* vector) {
    cout << step + 1 << "\t";
    for (size_t i = 0; i < params.k; i++) {
        cout << vector[i] << "\t";
    }
}

void PrintHeader(const PARAMS & params) {
    cout << "N\t";
    for (size_t i = 0; i < params.k; i++) {
        cout << "X" << i + 1 << "\t";
    }
    for (size_t i = 0; i < params.l; i++) {
        cout << "Z" << i + 1 << "\t";
    }
    cout << endl;
}

void PrintFooter(const PARAMS & params, double average, double variance, size_t step) {
    for (size_t j = 0; j < params.k; j++) {
        cout << "\t";
    }
    cout << "Zavg[" << step + 1 << "]";
    for (size_t j = 0; j < step + 1; j++) {
        cout << "\t";
    }
    cout << average << endl;
    for (size_t j = 0; j < params.k; j++) {
        cout << "\t";
    }
    cout << "S^2 [" << step + 1 << "]";
    for (size_t j = 0; j < step + 1; j++) {
        cout << "\t";
    }
    cout << variance << endl;
}

void PrintResearchHeader() {
    cout << endl;
    cout << "N" << "\t" << "Ai  " << "\t" << "Si^2" << endl;
}

void PrintResearchFooter(double Aicur, double variance, size_t step) {
    cout << step << "\t" << Aicur << "\t" << variance << endl;
}

void GetSensorsData(const PARAMS & params, Plant plant, double*** pxx) {
    *pxx = new double* [params.N];
    for (size_t i = 0; i < params.N; i++) {
        (*pxx)[i] = new double [params.k];
        GetCurrentVector(params, plant, &((*pxx)[i]));
        // cout << i << " GOD DAMNED" << endl;
        Sleep(params.T);
    }
}

void DisposeSensorData(const PARAMS & params, double*** pxx) {
        if (*pxx) {
        for (size_t i = 0; i < params.N; i++) {
            if ((*pxx)[i]) {
                delete (*pxx)[i];
                (*pxx)[i] = 0;
            }
        }
        delete *pxx;
        *pxx = 0;
    }
}
