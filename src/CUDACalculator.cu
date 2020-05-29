//
// Created by Guannan Guo on 5/27/20.
//

#include "CUDACalculator.h"
#include <iostream>

// define hardware dependent variables
static const int blockSize = 1024;
static const int gridSize = 24; //this number is hardware-dependent; usually #SM*2 is a good number.

__global__ void sumCommMultiBlock(const double *gArr, int arraySize, double *gOut) {
    int thIdx = threadIdx.x;
    int gthIdx = thIdx + blockIdx.x*blockSize;
    const int gridSize = blockSize*gridDim.x;
    double sum = 0;
    for (int i = gthIdx; i < arraySize; i += gridSize)
        sum += gArr[i];
    __shared__ double shArr[blockSize];
    shArr[thIdx] = sum;
    __syncthreads();
    for (int size = blockSize/2; size>0; size/=2) { //uniform
        if (thIdx<size)
            shArr[thIdx] += shArr[thIdx+size];
        __syncthreads();
    }
    if (thIdx == 0)
        gOut[blockIdx.x] = shArr[0];
}

double sumArray(double* roi_pointer, int arraySize) {
//    int* roi_in;
//    cudaMalloc(&roi_in, arraySize);
//    cudaMemcpy(roi_in, roi_domain.data, arraySize, cudaMemcpyHostToDevice);
    // setting cache and shared modes
    cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);
    cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);
    // transfer data from host to device
    double* img_in;
    cudaMalloc(&img_in, arraySize * sizeof(double));
    cudaMemcpy(img_in, roi_pointer, arraySize * sizeof(double), cudaMemcpyHostToDevice);

    double roi_sum;
    double* img_out;
    cudaMalloc((void**)&img_out, sizeof(double)*gridSize);
    // call the sum function
    sumCommMultiBlock<<<gridSize, blockSize>>>(img_in, arraySize, img_out);
    sumCommMultiBlock<<<1, blockSize>>>(img_out, gridSize, img_out);
    cudaDeviceSynchronize();
    // transfer data from device to host
    cudaMemcpy(&roi_sum, img_out, sizeof(double), cudaMemcpyDeviceToHost);
    cudaFree(img_in);
    cudaFree(img_out);
    return roi_sum;
}