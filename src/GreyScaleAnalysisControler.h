/*
 * GreyScaleAnalysisControler.h
 *
 *  Created on: May 2, 2020
 *      Author: Grant
 */

#ifndef SRC_GREYSCALEANALYSISCONTROLER_H_
#define SRC_GREYSCALEANALYSISCONTROLER_H_
class GreyScaleAnalysisControler {
private:
	Mat origins;
	Mat dimensions;
	char* path;
	char* file_format;
public:
	GreyScaleAnalysisControler(Mat origins_in, Mat dimensions_in, char* path_in, char* file_format_in);
	~GreyScaleAnalysisControler();
	void print2Console();
	void write2CSV(Mat data_in);
	string get_filename(int index_in, string file_format_in);
	int get_filenumber(char* path_in, char* file_format_in);
	Mat get_histogram_data();
	Mat open_image(string filename);
};
#endif /* SRC_GREYSCALEANALYSISCONTROLER_H_ */
