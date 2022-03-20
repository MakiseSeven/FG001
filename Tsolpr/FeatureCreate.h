#pragma once
#include"config.h"
#include"opencv2/opencv.hpp"
using namespace cv;

namespace Tsolpr {
	Mat getHistogram(Mat in);

	void getHistogramFeatures(const Mat& image, Mat& features);

	void FeatureCreate(const Mat& image, Mat& features);

	void getColorFeatures(const Mat& src, Mat& features);

	Mat CharFeatureCreate(const Mat& in, int sizeData);

	void ExtractCharFeature(const Mat& grayChar, Mat& features);

	Rect CenterRect(Mat& in);

	Mat CutTheRect(Mat& in, Rect& rect);

	Mat AlphaCharFeatures(Mat in, int sizeData);
}