
#include <opencv2/opencv.hpp>
#include <chrono>
#include "InputFileReader.h"
#include "GPUWarmer.h"
#include "GreyScaleCalculator.h"
#include "GreyScaleAnalysisControler.h"
using namespace cv;
using namespace std;
int main( int argc, char** argv )
{
    // warm up the GPU
    std::chrono::steady_clock::time_point warmup_begin = std::chrono::steady_clock::now();
    GPUWarmer gpuWarmer;
    gpuWarmer.warmUp();
    std::chrono::steady_clock::time_point warmup_end = std::chrono::steady_clock::now();
    std::cout << "Warm up GPU takes time (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(warmup_end - warmup_begin).count()) /1000000.0 <<std::endl;
    // start to count
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Mat origins = (Mat_<double>(2,2) << 20.0, 75.0, 190.0, 340.0);
    Mat dimensions = (Mat_<double>(2,2) << 290.0, 75.0, 90.0, 50.0);
    // Mat origins = (Mat_<double>(1,2) << 190.0, 340.0);
    // Mat dimensions = (Mat_<double>(1,2) << 90.0, 50.0);
    // Mat origins = (Mat_<double>(1,2) << 20.0, 75.0);
    // Mat dimensions = (Mat_<double>(1,2) << 290.0, 75.0);
	GreyScaleAnalysisControler controler(origins, dimensions, "../ImageData", "bmp");
	//controler.print2Console();
	Mat histData = controler.get_histogram();
//	cout << histData.cols << endl;
//	cout << histData.rows << endl;
	//cout << histData << endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Actual calculation takes time (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;
    controler.write2CSV(histData);
}
