#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
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
	//cout<< "x: " << this->origins.at<double>(1, 0) <<" y: " << this->origins.at<double>(1, 1) << endl;
	//cout<< "x: " << this->origins.at<double>(2, 0) <<" y: " << this->origins.at<double>(2, 1) << endl;
	cout<< "print dimensions" <<endl;
	cout<< "x: " << this->dimensions.at<double>(0, 0) <<" y: " << this->dimensions.at<double>(0, 1) << endl;
	//cout<< "x: " << this->dimensions.at<double>(1, 0) <<" y: " << this->dimensions.at<double>(1, 1) << endl;
	//cout<< "x: " << this->dimensions.at<double>(2, 0) <<" y: " << this->dimensions.at<double>(2, 1) << endl;
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
	myfile.open ("../Out/result.csv");
	for(int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			myfile << to_string(data_in.at<double>(i,j)) + ",";
		}
		myfile << "\n";
	}
	myfile.close();
}

Mat GreyScaleAnalysisControler::open_image(string filename) {
	Mat image;
	image = imread(filename);
	return image;
}

Mat GreyScaleAnalysisControler::writeHistDataToArray(int i_ROI) {
	int n_files = this->get_filenumber(this->path, this->file_format);
	double origin[2] = {this->origins.at<double>(i_ROI,0), this->origins.at<double>(i_ROI,1)};
	double dimension[2] = {this->dimensions.at<double>(i_ROI,0), this->dimensions.at<double>(i_ROI,1)};
	Size histDataSize(n_files, dimension[0] * dimension[1]);
	Mat histData(dimension[0]*dimension[1], 1, CV_8UC3);
	//cout << histData.cols << "\t" << histData.rows << endl;
	//cout << "origin: " << origin[0] << "\t" << origin[1] << endl;
	//cout << "dimension: " << dimension[0] << "\t" << dimension[1] << endl;
	//cout << "-----------" << endl;
	Rect ROI(origin[0], origin[1], dimension[0], dimension[1]); 
	Size size(1, dimension[0] * dimension[1]);
	for(int i = 0; i < n_files; i++) {
		string filename = "../ImageData/" + this->get_filename(i, this->file_format);
		Mat image_proc = this->open_image(filename);
		Mat sub_image(image_proc, ROI);
		if(!sub_image.isContinuous()) {
			sub_image = sub_image.clone();
		}
		sub_image = sub_image.reshape(0, (int)dimension[0] * (int)dimension[1]);
		cv::hconcat(histData, sub_image, histData);
		//resize(sub_image, resize_image, size, cv::INTER_NEAREST);
		//cout << "Info" << endl;
		//cout << sub_image.at<Vec3b>(0, 0) << endl;
		//cout << histData.cols << "\t" << histData.rows << endl;
		//cout << "-------------------" << endl;	
	}
	return histData;
}

Mat GreyScaleAnalysisControler::get_histogram_data() {
	int n_origins = this->origins.rows;
	int n_files = this->get_filenumber(this->path, this->file_format);
	cout << "n_origins: " << n_origins << endl;
	cout << "n_files: " << n_files << endl;
	Mat total_hist_data(1, n_files + 1, CV_8UC3);
	for(int i = 0; i < n_origins; i++) {
		double origin[2] = {this->origins.at<double>(i, 0), this->origins.at<double>(i, 1)};
		double dimension[2] = {this->dimensions.at<double>(i, 0), this->dimensions.at<double>(i, 1)};
		Mat sub_mat = this->writeHistDataToArray(i);
		GreyScaleCalculator calculator(origin, dimension);
		Mat sub_hist_data = calculator.calc_greyscale(sub_mat);
		cv::vconcat(total_hist_data, sub_hist_data, total_hist_data);
	}
	return total_hist_data;
}



