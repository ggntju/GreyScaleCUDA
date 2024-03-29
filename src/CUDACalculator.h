//
// Created by Guannan Guo on 5/27/20.
//

#ifndef GREYSCALECUDA_CUDACALCULATOR_H
#define GREYSCALECUDA_CUDACALCULATOR_H
#pragma once

#include <cstdint>
#include "cuda_runtime.h"

#ifdef __INTELLISENSE__
#define asm(x)
#define min(x) 0
#define fmaf(x) 0
#include "device_launch_parameters.h"
#define __CUDACC__
#include "device_functions.h"
#undef __CUDACC__
#endif
double sumArray(double* roi_pointer, int arraySize);
#endif //GREYSCALECUDA_CUDACALCULATOR_H
