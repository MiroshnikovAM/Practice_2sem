#include "practice.h"

using namespace std;

int main() {
    PARAMS params;
    // GetParams(params);
    GetTestParams(params);
    Plant plant;
    plant_init(plant);
    char* filename = "data.raw";

    PrintResearchHeader();
    size_t step = 0;
    double** zz = 0;
    for (double Aicur = params.Aimin; Aicur < params.Aimax; Aicur += params.dAi) {
        double* x = 0;
        double* z = 0;
        // PrintHeader(params);
        for (size_t i = 0; i < params.N; i++) {
            GetCurrentVector(params, plant, &x);
            GetCalculatedVector(params, x, &z);
            // PrintCurrentVector(params, i, x);
            WriteCalculatedVector(params, filename, i, z);
            Sleep(params.T);
        }
        DisposeVector(&x);
        DisposeVector(&z);

        ReadCalculatedVectors(params, filename, &zz);
        double average;
        double variance;
        /*
        for (size_t i = 0; i < params.l; i++) {
            average = CalculateAverage(params, zz[i]);
            variance = CalculateVariance(params, average, zz[i]);
            // PrintFooter(params, average, variance, i);
        }
        */
        params.A[0][params.researchCoeff] = Aicur;
        average = CalculateAverage(params, zz[0]);
        variance = CalculateVariance(params, average, zz[0]);
        PrintResearchFooter(Aicur, variance, step + 1);
        step++;
    }
    DisposeMatrix(params, &zz);
    DisposeParams(params);
    return 0;
}
