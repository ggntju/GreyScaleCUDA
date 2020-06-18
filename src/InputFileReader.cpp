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
//    cout << "Print contents from File reader" << endl;
//    for (int i = 0; i < 4; i++) {
//        cout << row[i] << endl;
//    }
    Mat origins(row);
    origins.reshape(0, 2);
    cout << "origins: " << origins << endl;
    cout << "size of origins: " << origins.size << endl;
    return origins;
}

Mat InputFileReader::readDimensions() {
    Mat dimensions;

    return dimensions;
}