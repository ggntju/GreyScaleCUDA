#ifndef GREY_SCALE_CALCULATOR_H
#define GREY_SCALE_CALCULATOR_H

using namespace std;
using namespace cv;
class GreyScaleCalculator {
private:
	double origin[2];
	double dimension[2];
public:
	GreyScaleCalculator(double* origin_in, double* dimension_in);
	~GreyScaleCalculator();
	void printInfo();
	Mat calc_greyscale(Mat image_in);
};
#endif
