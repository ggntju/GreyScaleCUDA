//
// Created by Guannan Guo on 5/27/20.
//

#include "CUDACalculator.h"

__global__ void cuda_calculate_greyscale_kernel(const cudaTextureObject_t tex_img, const int pixels, int* const res) {
    const unsigned int x = (blockIdx.x << 8) + threadIdx.x;
    const uint8_t sub_res = tex1Dfetch<int>(tex_img, 3 * x) + tex1Dfetch<int>(tex_img, 3 * x + 1) + tex1Dfetch<int>(tex_img, 3 * x + 2);
    if (x < pixels) {
        res[0] = res[0] + sub_res;
    }
}

void cuda_calculate_greyscale(const cudaTextureObject_t tex_img, const int pixels, int* const res) {
    cuda_calculate_greyscale_kernel<<<((pixels - 1) >> 8) + 1, 256>>>(tex_img, pixels, res);
    cudaDeviceSynchronize();
}

