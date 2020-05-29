
#include <opencv2/opencv.hpp>
#include <chrono>
#include "GreyScaleCalculator.h"
#include "GreyScaleAnalysisControler.h"
using namespace cv;
using namespace std;
int main( int argc, char** argv )
{

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //Mat origins = (Mat_<double>(2,2) << 20.0, 75.0, 190.0, 340.0);
    //Mat dimensions = (Mat_<double>(2,2) << 290.0, 75.0, 90.0, 50.0);
    Mat origins = (Mat_<double>(1,2) << 20.0, 75.0);
    Mat dimensions = (Mat_<double>(1,2) << 290.0, 75.0);
	GreyScaleAnalysisControler controler(origins, dimensions, "../ImageData", "bmp");
	controler.print2Console();
	Mat histData = controler.get_histogram();
	cout << histData.cols << endl;
	cout << histData.rows << endl;
	//cout << histData << endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;
    controler.write2CSV(histData);
}
