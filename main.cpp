#include <iostream>
#include <vector>
#include "plant.h"

using namespace std;

struct INPUT_DATA {
    size_t k;
    size_t* M;
    size_t T;
    size_t N;
    size_t l;
    double** A;
};

void Fill_Data(INPUT_DATA & Data);
void Dispose_Data(INPUT_DATA & Data);

int main() {
    INPUT_DATA Input_Data;
    Fill_Data(Input_Data);
    return 0;
}

void Fill_Data(INPUT_DATA & Data) {
    Data.k = 4;
    Data.M = new size_t [Data.k];
    for (size_t i = 0; i < Data.k; i++) {
        Data.M[i] = i;
    }
    Data.T = 10;
    Data.N = 5;
    Data.l = 2;
    Data.A = new double* [Data.l];
    for (size_t i = 0; i < Data.l; i++) {
        Data.A[i] = new double [Data.k+1];
        for (size_t j = 0; j < Data.k+1; j++) {
            Data.A[i][j] = 1;
        }
    }
}

void Dispose_Data(INPUT_DATA & Data) {
    if (Data.M) {
        delete Data.M;
        Data.M = 0;
    }
    if (Data.A) {
        for (size_t i = 0; i < Data.l; i++) {
            if (Data.A[i]) {
                delete Data.A[i];
                Data.A[i] = 0;
            }
        }
        delete Data.A;
        Data.A = 0;
    }
}
