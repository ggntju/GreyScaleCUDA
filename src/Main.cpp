
#include <opencv2/opencv.hpp>
#include <chrono>
#include "GreyScaleCalculator.h"
#include "GreyScaleAnalysisControler.h"
using namespace cv;
using namespace std;
int main( int argc, char** argv )
{
	/*
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	string filename = "../ImageData/00053.bmp";
	double origin[2] = {20.0, 75.0};
	double dimension[2] = {290.0, 75.0};

	double origin2[2] = {190.0, 340.0};
	double dimension2[2] = {90.0, 50.0};

	GreyScaleCalculator calc(filename, origin, dimension);
	GreyScaleCalculator calc2(filename, origin2, dimension2);
	Mat image_proc = calc.open_image();
	Mat image_proc2 = calc2.open_image();
	calc.calc_greyscale(image_proc);
	calc2.calc_greyscale(image_proc2);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;
	*/

	/*
	Mat C = (Mat_<double>(3,3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
	cout << "C = " << endl << " " << C << endl << endl;
	for(int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << "i: " << i << endl;
			cout << "j: " << j << endl;
			cout << "value: " << C.at<double>(i, j) << endl;
			cout << "------------" << endl;
		}
	}
	*/

	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Mat origins = (Mat_<double>(1,2) << 20.0, 75.0);
	Mat dimensions = (Mat_<double>(1,2) << 290.0, 75.0);
	GreyScaleAnalysisControler controler(origins, dimensions, "../ImageData", "bmp");
	controler.print2Console();
	Mat histData = controler.get_histogram_data();
	cout << histData.cols << endl;
	cout << histData.rows << endl;
	//cout << histData << endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;
	
}

