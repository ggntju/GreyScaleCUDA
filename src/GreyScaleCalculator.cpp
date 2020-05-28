#include <opencv2/opencv.hpp>
#include "GreyScaleCalculator.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CUDACalculator.h"
using namespace cv;
using namespace std;

GreyScaleCalculator::GreyScaleCalculator(string filename_in, double* origin_in, double* dimension_in) {
	filename = filename_in;
	for(int i = 0; i < 2; i++) {
		origin[i] = origin_in[i];
		dimension[i] = dimension_in[i];
	}
}
GreyScaleCalculator::~GreyScaleCalculator() {

}

void GreyScaleCalculator::printInfo() {
	cout<<"filename: "<< this->filename << endl;
	cout<<"origin: " << this->origin[0] << "\t" << this->origin[1] << endl;
	cout<<"dimension: "<< this->dimension[0] << "\t" << this->dimension[1] <<endl;
}

Mat GreyScaleCalculator::open_image() {
	Mat image;
	image = imread(this->filename);
	// cout << this->filename << " opened!" <<endl;
	return image;
}

Scalar GreyScaleCalculator::get_pixel(Mat image_in, int i, int j) {
	Scalar pixel;
	int x1 = this->origin[0];
	int y1 = this->origin[1];
	int x2 = x1 + this->dimension[0];
	int y2 = y1 + this->dimension[1];
	if(i < x1 || i > x2 || j < y1 || j > y2) {
		cout << "Out of boundary!" << endl;
		cin.get();
		return NULL;
	}
	pixel = image_in.at<Vec3b>(Point(i,j));
	// cout<< "pixel at " << "\t" << i << "\t" << j <<endl;
	// cout<< pixel[0] << "\t" << pixel[1] << "\t" << pixel[2] << "\n" <<endl;
	return pixel;
}

double GreyScaleCalculator::calc_greyscale(Mat image_in) {
	double width = this->dimension[0];
	double height = this->dimension[1];
	//cout << "width: " << width << endl;
	//cout << "height: " << height << endl;
	double grey_sum = 0.0;
	double pixel_count = 0.0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Scalar pixel = this->get_pixel(image_in, this->origin[0] + i, this->origin[1] + j);
			double red = pixel.val[0];
			double green = pixel.val[1];
			double blue = pixel.val[2];
			//cout<< "red: " << red << endl;
			double grey = red * 0.299 + green * 0.587 + blue * 0.114;
			grey_sum = grey_sum + grey;
			pixel_count = pixel_count + 1.0;
		}
	}
	//cout<<"grey_sum: " << grey_sum << endl;
	//cout<<"pixel_count: " << pixel_count << endl;
	return grey_sum/pixel_count;
}

double GreyScaleCalculator::CUDA_greyscale() {
    Mat total_domain = this->open_image();
    Rect ROI(this->origin[0], this->origin[1], this->dimension[0], this->dimension[1]);
    Mat roi_domain(total_domain, ROI);

    // setting cache and shared modes
    cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);
    cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);

    // allocating and transferring image as texture object
    uint8_t* d_img;
    cudaMalloc(&d_img, 3*roi_domain.rows*roi_domain.cols);
    cudaMemcpy(d_img, roi_domain.data, 3*roi_domain.rows*roi_domain.cols, cudaMemcpyHostToDevice);
    struct cudaResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(resDesc));
    resDesc.resType = cudaResourceTypeLinear;
    resDesc.res.linear.devPtr = d_img;
    resDesc.res.linear.desc.f = cudaChannelFormatKindUnsigned;
    resDesc.res.linear.desc.x = 8;
    resDesc.res.linear.sizeInBytes = 3 * roi_domain.rows * roi_domain.cols;

    struct cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.addressMode[0] = texDesc.addressMode[1] = texDesc.addressMode[2] = texDesc.addressMode[3] = cudaAddressModeBorder;
    texDesc.filterMode = cudaFilterModePoint;
    texDesc.readMode = cudaReadModeElementType;
    texDesc.normalizedCoords = 0;
    cudaTextureObject_t tex_img = 0;
    cudaCreateTextureObject(&tex_img, &resDesc, &texDesc, nullptr);
    // Setup initial value
    uint8_t* pixel_sum;
    pixel_sum[0] = 0;
    cout << "pixel sum: " << pixel_sum[0] << endl;
    // Call CUDA function
    cuda_calculate_greyscale(tex_img, roi_domain.rows * roi_domain.cols, pixel_sum);
    return (double) pixel_sum[0]/ (double) (3 * roi_domain.rows * roi_domain.cols);
}

