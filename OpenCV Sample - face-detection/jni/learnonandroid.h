/*
 * svmclassification.h
 *
 *  Created on: 27 de set de 2015
 *      Author: allansp
 */

#ifndef LEARNONANDROID_H_
#define LEARNONANDROID_H_

#include <fstream>

#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "include/lbp-adapter.hpp"

#define STRIDE 16
#define BOX_SIZE 64
#define DEFAULT_CELLSIZE 64

using namespace cv;
using namespace std;

class LearnOnAndroid {

private:
	CvSVM SVM;
	bool has_setted_feature_vector;
	int stride;
	int box_size;
	int dimension_buffer;
	int dimension_histogram;

	VlLbp* m_lbp_model;
	int default_cellsize;


public:
	Mat input_image;
	string model;
	float* feature_vector;

public:
	LearnOnAndroid();

	LearnOnAndroid(Mat input_image, string model);

	virtual ~LearnOnAndroid();

	void set_dimension_histogram();

	void set_dimension_buffer();

	void set_lbp_model();

	void set_image(Mat image);

	void load_image(string image_filename);

	void set_classification_model(string model);


	void init_feature_vector();

	void set_feature_vector(int dimension);

	void extract_lbp_features(Mat& image);

	void save_feature(string output_filename);

	void scaning_image(Mat& result);

	int get_box_size() const {
		return this->box_size;
	}

	void set_box_size(int boxsize) {
		this->box_size = boxsize;
	}

	int get_default_cellsize() const {
		return this->default_cellsize;
	}

	void set_default_cellsize(int defaultcellsize) {
		this->default_cellsize = defaultcellsize;
	}

	int get_stride() const {
		return this->stride;
	}

	void set_stride(int stride) {
		this->stride = stride;
	}

private:

	void __load_vector(string filename, float* vector);

	void __normalize_feature_vector();

	void __delete_input_image();

	void __delete_feature_vector();

	void __set_feature_vector(int dimension);

	void __printing_feature_vector(float* vector);

	float __testing();

};

#endif /* LEARNONANDROID_H_ */
