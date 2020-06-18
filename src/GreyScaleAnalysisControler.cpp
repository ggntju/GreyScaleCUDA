#include <opencv2/opencv.hpp>
#include "GreyScaleCalculator.h"
#include "GreyScaleAnalysisControler.h"
#include <dirent.h>
#include <fstream>
using namespace cv;
using namespace std;

GreyScaleAnalysisControler::GreyScaleAnalysisControler(Mat origins_in, Mat dimensions_in, char* path_in, char* file_format_in) {
	this->origins = origins_in;
	this->dimensions = dimensions_in;
	this->path = path_in;
	this->file_format = file_format_in;
}
GreyScaleAnalysisControler::~GreyScaleAnalysisControler() {
	//free(this->path);
	//free(this->file_format);
}

void GreyScaleAnalysisControler::print2Console() {
	cout<< "print2Console method called" << endl;
	cout<< "print origins" <<endl;
	cout<< "x: " << this->origins.at<double>(0, 0) <<" y: " << this->origins.at<double>(0, 1) << endl;
	cout<< "x: " << this->origins.at<double>(1, 0) <<" y: " << this->origins.at<double>(1, 1) << endl;
	//cout<< "x: " << this->origins.at<double>(2, 0) <<" y: " << this->origins.at<double>(2, 1) << endl;
	cout<< "print dimensions" <<endl;
	cout<< "x: " << this->dimensions.at<double>(0, 0) <<" y: " << this->dimensions.at<double>(0, 1) << endl;
	cout<< "x: " << this->dimensions.at<double>(1, 0) <<" y: " << this->dimensions.at<double>(1, 1) << endl;
	//cout<< "x: " << this->dimensions.at<double>(2, 0) <<" y: " << this->dimensions.at<double>(2, 1) << endl;
}

Mat GreyScaleAnalysisControler::get_histogram() {
	int n_origins = this->origins.rows;
	int n_files = this->get_filenumber(this->path, this->file_format);
	cout << to_string(n_files) + " files found!" << endl;
	cout << to_string(n_origins) + " ROIs found!" << endl;
	double histogram_data[n_files][n_origins];
	for(int i = 0; i < n_files; i++) {
		//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (int j = 0; j < n_origins; j++) {
			string filename = this->get_filename(i, this->file_format);
			//cout << "Processing " + filename + " ROI " + to_string(j) << endl;
			double origin[2] = {this->origins.at<double>(j,0), this->origins.at<double>(j,1)};
			double dimension[2] = {this->dimensions.at<double>(j,0), this->dimensions.at<double>(j,1)};
			GreyScaleCalculator calculator("../ImageData/" + filename, origin, dimension);
			double calc_result = calculator.CUDA_greyscale();
			histogram_data[i][j] = calc_result;
		}
		//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		//std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;
	}
	Mat result(n_files, n_origins, CV_64F);
	std::memcpy(result.data, histogram_data, n_files*n_origins*sizeof(double));
	return result;
}

string GreyScaleAnalysisControler::get_filename(int index_in, string file_format_in) {
	string filename = "filename." + file_format_in;
	if(index_in >= 0 && index_in <10) {
		filename = "0000" + to_string(index_in) + "." + file_format_in;
		return filename;
	}
	if(index_in >= 10 && index_in <100) {
		filename = "000" + to_string(index_in) + "." + file_format_in;
		return filename;
	}
	if(index_in >= 100 && index_in <1000) {
		filename = "00" + to_string(index_in) + "." + file_format_in;
		return filename;
	}
	if(index_in >= 1000 && index_in <10000) {
		filename = "0" + to_string(index_in) + "." + file_format_in;
		return filename;
	}
	if(index_in >= 10000) {
		filename = to_string(index_in) + "." + file_format_in;
		return filename;
	}
	return filename;
}

int GreyScaleAnalysisControler::get_filenumber(char* path_in, char* file_format_in) {
	int count = 0;
	DIR *di;
	char *ptr1,*ptr2;
	int retn;
	struct dirent *dir;
	di = opendir(path_in); //specify the directory name
	if (di) {
		while ((dir = readdir(di)) != NULL) {
			ptr1=strtok(dir->d_name,".");
	        ptr2=strtok(NULL,".");
	        if(ptr2!=NULL) {
	        	retn=strcmp(ptr2,file_format_in);
	            if(retn==0) {

	            	count++;
	            }
	            //free(ptr2);
	        }
	        //free(ptr1);
	        //free(dir);
	    }
	    closedir(di);
	    //free(di);
	}
	return count;
}

void GreyScaleAnalysisControler::write2CSV(Mat data_in) {
	int rows = data_in.rows;
	int cols = data_in.cols;
	std::ofstream myfile;
	myfile.open ("../Output/result.csv");
	for(int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			myfile << to_string(data_in.at<double>(i,j)) + ",";
		}
		myfile << "\n";
	}
	myfile.close();
}

