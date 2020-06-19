//
// Created by Guannan Guo on 6/17/20.
//

#include <opencv2/opencv.hpp>
#include <fstream>
#include "InputFileReader.h"

using namespace cv;
using namespace std;

Mat InputFileReader::readOrigins() {
    // File pointer
    fstream fin;
    // Open existing file
    fin.open("../Input/origins.csv", ios::in);
    // Read the data from file as string vector
    vector<double> row;
    string line, word;
    // Clear it first
    row.clear();
    // Read the entire row
    getline(fin, line);
    // Used for breaking words
    stringstream s(line);
    // Read data by column
    while (getline(s, word, ',')) {
        row.push_back(stod(word));
    }
    Mat_<double> origins(row);
    cout << "Origins in file reader: " << "\n" << origins << endl;
    origins = origins.reshape(0, 2);
    cout << "Origins after reshape: " << "\n" << origins << endl;
    // Mat origins = (Mat_<double>(2,2) << 20.0, 75.0, 190.0, 340.0);
    return origins;
}

Mat InputFileReader::readDimensions() {
    // File pointer
    fstream fin;
    // Open existing file
    fin.open("../Input/dimensions.csv", ios::in);
    // Read the data from file as string vector
    vector<double> row;
    string line, word;
    // Clear it first
    row.clear();
    // Read the entire row
    getline(fin, line);
    // Used for breaking words
    stringstream s(line);
    // Read data by column
    while (getline(s, word, ',')) {
        row.push_back(stod(word));
    }
//    Mat dimensions(row);
//    cout << "Dimensions in file reader: " << "\n" << dimensions << endl;
//    dimensions = dimensions.reshape(0, 2);
//    cout << "Dimensions after reshape: " << "\n" << dimensions << endl;
    Mat dimensions = (Mat_<double>(2,2) << 290.0, 75.0, 90.0, 50.0);
    return dimensions;
}