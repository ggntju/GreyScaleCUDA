#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include "GreyScaleCalculator.h"
using namespace cv;
using namespace std;

GreyScaleCalculator::GreyScaleCalculator(double* origin_in, double* dimension_in) {
	for(int i = 0; i < 2; i++) {
		origin[i] = origin_in[i];
		dimension[i] = dimension_in[i];
	}
}
GreyScaleCalculator::~GreyScaleCalculator() {

}

void GreyScaleCalculator::printInfo() {
	cout<<"origin: " << this->origin[0] << "\t" << this->origin[1] << endl;
	cout<<"dimension: "<< this->dimension[0] << "\t" << this->dimension[1] <<endl;
}

Mat GreyScaleCalculator::calc_greyscale(Mat image_in) {
	Mat histArray;

	return histArray;      
}

