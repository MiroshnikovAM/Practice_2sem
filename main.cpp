#include "practice.h"

using namespace std;

int main() {
    PARAMS params;
    // GetParams(params);
    GetTestParams(params);
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
        WriteCalculatedVector(params, filename, i, z);
        Sleep(params.T);
    }
    DisposeVector(&x);
    DisposeVector(&z);

    double** zz = 0;
    ReadCalculatedVectors(params, filename, &zz);
    double average;
    double variance;
    // Main Part
    /*
    for (size_t i = 0; i < params.l; i++) {
        average = CalculateAverage(params, zz[i]);
        variance = CalculateVariance(params, average, zz[i]);
        PrintFooter(params, average, variance, i);
    }
    */
    // Research Part
    PrintResearchHeader();
    size_t i = 0;
    for (double Aicur = params.Aimin; Aicur < params.Aimax; Aicur += params.dAi) {
        params.A[0][params.researchCoeff] = Aicur;
        average = CalculateAverage(params, zz[i]);
        variance = CalculateVariance(params, average, zz[i]);
        PrintResearchFooter(Aicur, variance, i + 1);
        i++;
    }
    DisposeMatrix(params, &zz);
    DisposeParams(params);
    return 0;
}
