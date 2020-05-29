#include <opencv2/opencv.hpp>
#include "GreyScaleCalculator.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CUDACalculator.h"
using namespace cv;
using namespace std;

GreyScaleCalculator::GreyScaleCalculator(string filename_in, double* origin_in, double* dimension_in) {
    //cout << "Calculator was constructed!" << endl;
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
//    int arraySize = roi_domain.rows * roi_domain.cols * 3;
//    int* roi_pointer;
//    cudaMalloc(&roi_pointer, arraySize);
//    cudaMemcpy(roi_pointer, roi_domain.data, arraySize, cudaMemcpyHostToDevice);
    // call cuda function
    int roi_sum = sumArray(roi_domain, arraySize);
    //cudaFree(roi_pointer);
    return (double)roi_sum/(double)arraySize;
}

