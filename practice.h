#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include "plant.h"

struct PARAMS {
    size_t k;
    size_t* M;
    size_t T;
    size_t N;
    size_t l;
    double** A;
    // research part
    size_t researchCoeff;
    double Aimin;
    double Aimax;
    double dAi;
};

void GetParams(PARAMS & params);
void GetTestParams(PARAMS & params);
void DisposeParams(PARAMS & params);
void DisposeVector(double** ptrVector);
void DisposeMatrix(const PARAMS & params, double*** ptrMatrix);
void GetCurrentVector(const PARAMS & params, Plant plant, double** ptrVector);
void GetCalculatedVector(const PARAMS & params, double* x, double** pZ);
void WriteCalculatedVector(const PARAMS & params, char* filename, size_t step, double* z);
void ReadCalculatedVectors(const PARAMS & params, char* filename, double*** pZZ);
double CalculateAverage(const PARAMS & params, double* vector);
double CalculateVariance(const PARAMS & params, double average, double* vector);
void PrintCurrentVector(const PARAMS & params, size_t step, double* vector);
void PrintHeader(const PARAMS & params);
void PrintFooter(const PARAMS & params, double average, double variance, size_t step);
void PrintResearchHeader();
void PrintResearchFooter(double Aicur, double variance, size_t step);
void GetSensorsData(const PARAMS & params, Plant plant, double*** pxx);
void DisposeSensorData(const PARAMS & params, double*** pxx);
