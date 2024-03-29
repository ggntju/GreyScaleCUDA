//
// Created by Guannan Guo on 5/29/20.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include "GPUWarmer.h"
#include "CUDARGB2Y.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

GPUWarmer::GPUWarmer() {

}

void GPUWarmer::warmUp() {
    // ------------- Configuration ------------
    constexpr auto warmups = 200;
    constexpr bool weighted_averaging = true;
    constexpr char name[] = "test.jpg";
    // --------------------------------


    // ------------- Image Read ------------
    cv::Mat image = cv::imread(name);
    if (!image.data) {
        std::cerr << "ERROR: failed to open image. Aborting." << std::endl;
    }
    // --------------------------------

    // setting cache and shared modes
    cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);
    cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);
    // allocating and transferring image as texture object
    uint8_t* d_img;
    cudaMalloc(&d_img, 3*image.rows*image.cols);
    cudaMemcpy(d_img, image.data, 3*image.rows*image.cols, cudaMemcpyHostToDevice);
    struct cudaResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(resDesc));
    resDesc.resType = cudaResourceTypeLinear;
    resDesc.res.linear.devPtr = d_img;
    resDesc.res.linear.desc.f = cudaChannelFormatKindUnsigned;
    resDesc.res.linear.desc.x = 8;
    resDesc.res.linear.sizeInBytes = 3 * image.rows * image.cols;
    struct cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.addressMode[0] = texDesc.addressMode[1] = texDesc.addressMode[2] = texDesc.addressMode[3] = cudaAddressModeBorder;
    texDesc.filterMode = cudaFilterModePoint;
    texDesc.readMode = cudaReadModeElementType;
    texDesc.normalizedCoords = 0;
    cudaTextureObject_t tex_img = 0;
    cudaCreateTextureObject(&tex_img, &resDesc, &texDesc, nullptr);
    // allocating space for new image
    uint8_t* d_newimg;
    cudaMalloc(&d_newimg, image.rows*image.cols);
    // Warming up
    std::cout << std::endl << "Warming up..." << std::endl;
    for (int i = 0; i < warmups; ++i) CUDARGB2Y(weighted_averaging, tex_img, image.cols*image.rows, d_newimg);
}
