#ifndef LBP_H_
#define LBP_H_

//! \author philipp <bytefish[at]gmx[dot]de>
//! \copyright BSD, see LICENSE.

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <limits>

using namespace cv;
using namespace std;

namespace lbp {

// templated functions
template <typename _Tp>
void OLBP_(const cv::Mat& src, cv::Mat& dst);

template <typename _Tp>
void ELBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);

template <typename _Tp>
void VARLBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);

// wrapper functions
void OLBP(const Mat& src, Mat& dst);
void ELBP(const Mat& src, Mat& dst, int radius = 1, int neighbors = 8);
void VARLBP(const Mat& src, Mat& dst, int radius = 1, int neighbors = 8);

// Mat return type functions
Mat OLBP(const Mat& src);
Mat ELBP(const Mat& src, int radius = 1, int neighbors = 8);
Mat VARLBP(const Mat& src, int radius = 1, int neighbors = 8);

}
#endif