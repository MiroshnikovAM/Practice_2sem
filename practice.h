#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include "plant.h"

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
void WriteCalculatedVector(const INPUT_DATA & data, char* filename, bool shouldAdd, double* z);
void ReadCalculatedVectors(const INPUT_DATA & data, char* filename, double*** pZZ);
double CalculateAverage(const INPUT_DATA & data, double* vector);
double CalculateVariance(const INPUT_DATA & data, double average, double* vector);
