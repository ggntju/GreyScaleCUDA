//
// Created by Guannan Guo on 6/17/20.
//

#ifndef GREYSCALECUDA_INPUTFILEREADER_H
#define GREYSCALECUDA_INPUTFILEREADER_H

using namespace std;
using namespace cv;

class InputFileReader {
public:
    Mat readOrigins();
    Mat readDimensions();
};


#endif //GREYSCALECUDA_INPUTFILEREADER_H
