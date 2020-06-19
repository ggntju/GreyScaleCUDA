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
    Mat origins(row);
    cout << "Origins in file reader: " << origins << endl;
    origins = origins.reshape(0, 2);
    cout << "Origins after reshape: " << origins << endl;
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
    Mat dimensions(row);
    cout << "Dimensions in file reader: " << dimensions << endl;
    dimensions = dimensions.reshape(0, 2);
    cout << "Dimensions after reshape: " << dimensions << endl;
    return dimensions;
}