//
// Created by Guannan Guo on 5/27/20.
//

#include "CUDACalculator.h"

// define hardware dependent variables
static const int blockSize = 1024;
static const int gridSize = 24; //this number is hardware-dependent; usually #SM*2 is a good number.

__global__ void sumCommMultiBlock(const long int *gArr, int arraySize, long int *gOut) {
    int thIdx = threadIdx.x;
    int gthIdx = thIdx + blockIdx.x*blockSize;
    const int gridSize = blockSize*gridDim.x;
    long int sum = 0;
    for (int i = gthIdx; i < arraySize; i += gridSize)
        sum += gArr[i];
    __shared__ long int shArr[blockSize];
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

long int sumArray(long int* roi_pointer, int arraySize) {
//    int* roi_in;
//    cudaMalloc(&roi_in, arraySize);
//    cudaMemcpy(roi_in, roi_domain.data, arraySize, cudaMemcpyHostToDevice);
    // setting cache and shared modes
    cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);
    cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);
    // transfer data from host to device
    long int* img_in;
    cudaMalloc(&img_in, arraySize * sizeof(long int));
    cudaMemcpy(img_in, roi_pointer, arraySize * sizeof(long int), cudaMemcpyHostToDevice);

    long int roi_sum;
    long int* img_out;
    cudaMalloc((void**)&img_out, sizeof(long int)*gridSize);
    // call the sum function
    sumCommMultiBlock<<<gridSize, blockSize>>>(img_in, arraySize, img_out);
    sumCommMultiBlock<<<1, blockSize>>>(img_out, gridSize, img_out);
    cudaDeviceSynchronize();
    // transfer data from device to host
    cudaMemcpy(&roi_sum, img_out, sizeof(long int), cudaMemcpyDeviceToHost);
    cudaFree(img_in);
    cudaFree(img_out);
    return roi_sum;
}