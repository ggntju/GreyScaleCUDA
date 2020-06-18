//
// Created by Guannan Guo on 6/17/20.
//

#include <opencv2/opencv.hpp>
#include <fstream>
#include "InputFileReader.h"

using namespace cv;
using namespace std;

Mat InputFileReader::readOrigins() {
    Mat origins;
    // File pointer
    fstream fin;
    // Open existing file
    fin.open("../Input/origins.csv", ios::in);
    // Read the data from file as string vector
    vector<string> row;
    string line, word;
    // Clear it first
    row.clear();
    // Read the entire row
    getline(fin, line);
    // Used for breaking words
    stringstream s(line);
    // Read data by column
    while (getline(s, word, ',')) {
        row.push_back(word);
    }
//    cout << "Print contents from File reader" << endl;
//    for (int i = 0; i < 4; i++) {
//        cout << row[i] << endl;
//    }
    origins(row);
    cout << "origins: " << origins << endl;
    return origins;
}

Mat InputFileReader::readDimensions() {
    Mat dimensions;

    return dimensions;
}