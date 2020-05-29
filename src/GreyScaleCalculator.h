#ifndef GREY_SCALE_CALCULATOR_H
#define GREY_SCALE_CALCULATOR_H

using namespace std;
using namespace cv;
class GreyScaleCalculator {
private:
	string filename;
	double origin[2];
	double dimension[2];
public:
	GreyScaleCalculator(string filename_in, double* origin_in, double* dimension_in);
	~GreyScaleCalculator();
	void printInfo();
	Mat open_image();
	Scalar get_pixel(Mat image_in, int i, int j);
	double calc_greyscale(Mat image_in);
	double CUDA_greyscale();
    void RGB2Y_ref(const uint8_t* __restrict const data, const int32_t cols, const int32_t rows, const int32_t stride, uint8_t* const __restrict out);
};
#endif
