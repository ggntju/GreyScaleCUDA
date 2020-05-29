//
// Created by Guannan Guo on 5/27/20.
//

#include "CUDACalculator.h"

// define hardware dependent variables
static const int blockSize = 1024;
static const int gridSize = 12; //this number is hardware-dependent; usually #SM*2 is a good number.

__global__ void sumCommMultiBlock(const int *gArr, int arraySize, int *gOut) {
    int thIdx = threadIdx.x;
    int gthIdx = thIdx + blockIdx.x*blockSize;
    const int gridSize = blockSize*gridDim.x;
    int sum = 0;
    for (int i = gthIdx; i < arraySize; i += gridSize)
        sum += gArr[i];
    __shared__ int shArr[blockSize];
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

int sumArray(Mat roi_in) {
    int arraySize = 3 * roi_in.rows * roi_in.cols;
    // setting cache and shared modes
    cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);
    cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);
    // transfer data from host to device
    int* img_in;
    cudaMalloc(&img_in, arraySize);
    cudaMemcpy(img_in, roi_in.data, arraySize, cudaMemcpyHostToDevice);

    int roi_sum;
    int* img_out;
    cudaMalloc((void**)&img_out, sizeof(int)*gridSize);
    // call the sum function
    sumCommMultiBlock<<<gridSize, blockSize>>>(img_in, arraySize, img_out);
    sumCommMultiBlock<<<1, blockSize>>>(img_out, gridSize, img_out);
    cudaDeviceSynchronize();
    // transfer data from device to host
    cudaMemcpy(&roi_sum, img_out, sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(img_in);
    cudaFree(img_out);
    return roi_sum;
}