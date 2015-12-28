/*
 * svmclassification.cpp
 *
 *  Created on: 27 de set de 2015
 *      Author: allansp
 */

#include "learnonandroid.h"

LearnOnAndroid::LearnOnAndroid() {

	this->set_lbp_model();

	this->stride = STRIDE;
	this->box_size = BOX_SIZE;
	this->dimension_buffer = 0;
	this->feature_vector = NULL;
	this->default_cellsize = DEFAULT_CELLSIZE;
	this->has_setted_feature_vector = false;

	this->set_dimension_histogram();

}

LearnOnAndroid::LearnOnAndroid(Mat input_image, string model) {

	this->set_lbp_model();
	this->set_image(input_image);
	this->set_classification_model(model);

	this->has_setted_feature_vector = false;

	this->stride = STRIDE;
	this->box_size = BOX_SIZE;
	this->default_cellsize = DEFAULT_CELLSIZE;

	this->set_dimension_histogram();
	this->set_dimension_buffer();

	this->set_feature_vector(this->dimension_buffer);
}

LearnOnAndroid::~LearnOnAndroid() {
	this->__delete_feature_vector();
}

void LearnOnAndroid::set_image(Mat image) {

	image.copyTo(this->input_image);
	cout << image.size() << endl;

}

void LearnOnAndroid::set_lbp_model(){
	m_lbp_model = vl_lbp_new(VlLbpUniform, false);
}

void LearnOnAndroid::load_image(string image_filename) {
	this->input_image = imread(image_filename, CV_LOAD_IMAGE_GRAYSCALE);
}

void LearnOnAndroid::set_classification_model(string model) {
	this->SVM.load(model.c_str());
}

void LearnOnAndroid::set_dimension_buffer() {
	this->dimension_buffer = floor(this->input_image.cols/this->get_default_cellsize()) *
					  	  	 floor(this->input_image.rows/this->get_default_cellsize()) *
							 vl_lbp_get_dimension(m_lbp_model);
}

void LearnOnAndroid::__delete_feature_vector() {

	if (this->dimension_buffer == 1){
		delete this->feature_vector;
	} else if (this->dimension_buffer > 1){
		delete[] this->feature_vector;
	}

	this->dimension_buffer = 0;
	this->has_setted_feature_vector = false;

}

void LearnOnAndroid::init_feature_vector() {

	if (this->has_setted_feature_vector){

		if (this->dimension_buffer == 1){
			this->feature_vector = 0;
		} else if (this->dimension_buffer > 1){
			for (int i=0; i < this->dimension_buffer; i++){
				this->feature_vector[i] = 0.0;
			}
		} else {
			cout << "Feature vector not setted yet!" << endl;
		}
	}
}

void LearnOnAndroid::__set_feature_vector(int dimension) {

	this->dimension_buffer = dimension;
	this->has_setted_feature_vector = true;

	if (this->dimension_buffer == 1) {

		this->feature_vector = new float();

	} else if (this->dimension_buffer > 1){

		this->feature_vector = new float[this->dimension_buffer];

	} else{

		exit(EXIT_FAILURE);

	}

}

void LearnOnAndroid::set_feature_vector(int dimension) {

	if (this->has_setted_feature_vector) {

		this->__delete_feature_vector();
		this->__set_feature_vector(dimension);

	} else{

		this->__set_feature_vector(dimension);

	}

}

void LearnOnAndroid::extract_lbp_features(Mat& image) {

	int m_img_width = image.cols;
	int m_img_height = image.rows;
	int sz = m_img_width * m_img_height;
	float* m_gray_data;

	this->init_feature_vector();

	m_gray_data = new float[sz];

	for(int i=0;i<image.rows; i++) {
		for(int j=0;j<image.cols; j++) {
			m_gray_data[i*image.rows + j] = (float)image.at<uchar>(i,j);
		}
	}

	vl_lbp_process(m_lbp_model, this->feature_vector, m_gray_data,
				   image.cols, image.rows, this->default_cellsize);

	delete[] m_gray_data;
}

void LearnOnAndroid::__delete_input_image() {
	this->input_image.release();
	cout << input_image.size() << endl;
}

void LearnOnAndroid::__normalize_feature_vector() {

	float* vector_mean;
	float* vector_std;

	vector_mean = new float[this->dimension_histogram];
	vector_std  = new float[this->dimension_histogram];

	this->__load_vector("/storage/sdcard0/mean.txt", vector_mean);
	this->__load_vector("/storage/sdcard0/std.txt", vector_std);

	for (int i = 0; i < this->dimension_histogram; i++) {
		this->feature_vector[i] -= vector_mean[i];
		this->feature_vector[i] /= vector_std[i];
	}

	delete[] vector_mean;
	delete[] vector_std;

}

void LearnOnAndroid::__load_vector(string filename, float* vector) {

	ifstream fin;

	fin.open(filename.c_str(), ios::in);
    fin.setf(ios::fixed,ios::floatfield);
//	fin.precision(12);

	float a;
	int i = 0;

	while (fin >> a) {
        vector[i] = (float) a;
        i++;
	}
}

void LearnOnAndroid::set_dimension_histogram() {
	this->dimension_histogram = 58*(this->box_size/this->get_default_cellsize())*
								   (this->box_size/this->get_default_cellsize());
}

float LearnOnAndroid::__testing() {

	Mat testing = Mat(1, this->dimension_histogram, CV_32FC1);

	for (int i=0; i<this->dimension_histogram;i++){
		testing.at<float>(i) = this->feature_vector[i];
	}

	return this->SVM.predict(testing);
}

void LearnOnAndroid::scaning_image(Mat& result) {

	Mat testing;
	Mat image_roi;

	vector<Point2i> points;
	Mat mask = Mat::zeros(this->input_image.size(), this->input_image.type());

	for (int r = 0; r < this->input_image.rows; r += this->stride) {
		for (int c = 0; c < this->input_image.cols; c += this->stride) {

			if (((r+this->box_size) < this->input_image.rows) &&
					((c+this->box_size) < this->input_image.cols)) {

				image_roi = this->input_image(Range(r, r+this->box_size),
						                      Range(c, c+this->box_size));

				this->extract_lbp_features(image_roi);

				this->__normalize_feature_vector();

				float response = this->__testing();

				cout << response << endl;

				if(response == 1.0){

					Point2i center = Point2i((c+this->box_size/2), (r+this->box_size/2));
					points.push_back(center);

					rectangle(mask, Point(c, r),
							  Point(c+this->box_size, r+this->box_size),
							  Scalar(255, 0, 0), CV_FILLED);

//					rectangle(result, Point(c, r),
//							  Point(c+this->box_size, r+this->box_size),
//							  Scalar(0, 0, 255));

				}
			}
		}
	}

	int sum_x = 0;
	int sum_y = 0;
	int size = points.size();
	Point2i centroid(this->input_image.cols/2, this->input_image.rows/2);

	if(size > 0){

	    for (int i = 0; i < size; i++) {
	    	Point2i p = points.back();
	        sum_x += p.x;
	        sum_y += p.y;
	    }

		centroid.x = sum_x/size;
		centroid.y = sum_y/size;

	}


//	rectangle(result,
//			  Point(centroid.x - this->box_size/2, centroid.y - this->box_size),
//			  Point(centroid.x + this->box_size/2, centroid.y + this->box_size/2 + 5),
//			  Scalar(0, 255, 0));
//
//	circle(result, centroid, 3, Scalar(0, 255, 0));

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

//	resize(mask, mask, Size(this->input_image.cols*2, this->input_image.rows*2), this->input_image.type());

	findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	vector<Point2f> centerofMass(contours.size());
	vector<Moments> mu(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		vector<Point> cont(contours[i]);
		double area = contourArea(cont);

		if ((area > 3000) && (area < 60000)) {

			drawContours(result, contours, i, Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point());
		}

	}

}

void LearnOnAndroid::save_feature(string output_filename) {

	ofstream fout;

	fout.open(output_filename.c_str(), ios::out | ios::trunc);
    fout.setf(ios::fixed,ios::floatfield);
//	fout.precision(12); // works fine only on PC

	for (int i = 0; i < this->dimension_histogram-1; i++) {

		if (isnanf((float) this->feature_vector[i]))
			fout << (float) 0.0 << ",";
		else
			fout << (float) this->feature_vector[i] << ",";

	}

	if (isnanf((float) this->feature_vector[this->dimension_histogram-1]))
		fout << (float) 0.0;
	else
		fout << (float) this->feature_vector[this->dimension_histogram-1];

	fout.close();

}

void LearnOnAndroid::__printing_feature_vector(float* vector) {

	cout << "this->dimension: " << this->dimension_histogram << endl;

	for (int i = 0; i < this->dimension_histogram-1; i++){
		cout << (float) vector[i] << ", ";
	}

	cout << (float) vector[this->dimension_histogram-1] << endl;
}
