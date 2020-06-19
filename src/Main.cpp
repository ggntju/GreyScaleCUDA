
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
    // read data from file
    InputFileReader inputFileReader;
    Mat origins = inputFileReader.readOrigins();
    Mat dimensions = inputFileReader.readDimensions();
    // display the input size to console
    cout << "Origins size: " << origins.size << endl;
    cout << "Dimensions size: " << dimensions.size << endl;
    // warm up the GPU
    std::chrono::steady_clock::time_point warmup_begin = std::chrono::steady_clock::now();
    GPUWarmer gpuWarmer;
    gpuWarmer.warmUp();
    std::chrono::steady_clock::time_point warmup_end = std::chrono::steady_clock::now();
    std::cout << "Warm up GPU takes time (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(warmup_end - warmup_begin).count()) /1000000.0 <<std::endl;
    // start to calculate time cost
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// create controler to get histogram data
	GreyScaleAnalysisControler controler(origins, dimensions, "../ImageData", "bmp");
	controler.print2Console();
	Mat histData = controler.get_histogram();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	// display the actual time cost to console
	std::cout << "Actual calculation takes time (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;
	// write the data into a CSV file
    controler.write2CSV(histData);
}
