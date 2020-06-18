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
    vector<float> row;
    string line, word;
    // Clear it first
    row.clear();
    // Read the entire row
    getline(fin, line);
    // Used for breaking words
    stringstream s(line);
    // Read data by column
    while (getline(s, word, ',')) {
        row.push_back(stof(word));
    }
    Mat origins(row);
    origins = origins.reshape(0, 2);
    return origins;
}

Mat InputFileReader::readDimensions() {
    // File pointer
    fstream fin;
    // Open existing file
    fin.open("../Input/dimensions.csv", ios::in);
    // Read the data from file as string vector
    vector<float> row;
    string line, word;
    // Clear it first
    row.clear();
    // Read the entire row
    getline(fin, line);
    // Used for breaking words
    stringstream s(line);
    // Read data by column
    while (getline(s, word, ',')) {
        row.push_back(stof(word));
    }
    Mat dimensions(row);
    dimensions = dimensions.reshape(0, 2);
    return dimensions;
}