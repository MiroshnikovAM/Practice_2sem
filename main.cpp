#include "practice.h"

using namespace std;

int main() {
    INPUT_DATA params;
    FillData(params);
    Plant plant;
    plant_init(plant);

    char* filename = "data.raw";

    double* x = 0;
    double* z = 0;

    for (size_t i = 0; i < params.N; i++) {
        GetCurrentVector(params, plant, &x);
        GetCalculatedVector(params, x, &z);
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
        cout << "Z_avg[" << i << "] = " << average << endl;
        cout << "S^2[" << i << "] = " << variance << endl;
    }
    DisposeMatrix(params, &zz);

    DisposeData(params);
    return 0;
}
