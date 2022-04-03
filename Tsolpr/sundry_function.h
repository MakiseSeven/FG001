#pragma once
#include"config.h"
#include"basic_plate.hpp"
namespace Tsolpr {

	bool verifySizes(RotatedRect mr);

	Mat ColoeMatch(const Mat& src, Mat& out, const Color color);

	bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv, float& percent);

	void TopBottomFound(Mat mask, int& top, int& bottom);

	void deleteNotArea(Mat& inMat, Color color = UNKNOWN);

	Color getPlateType(const Mat& src, const bool adaptive_minsv);

	Mat Resize(const Mat& result, float& scale);

	RotatedRect rectResize(const RotatedRect& rr, const float scale_ratio);

	Rect interRect(const Rect& a, const Rect& b);

	Rect outRect(const Rect& a, const Rect& b);

	float computeIOU(const Rect& rect1, const Rect& rect2);

	void NMS(std::vector<Plate>& inVec, std::vector<Plate>& resultVec, double overlap);

	void NMS(std::vector<Character>& inVec, double overlap);

	bool RectRevise(const RotatedRect& InRect_b, const Mat& src, Rect_<float>& Rect_Revise);

	float BlackPixelStatistic(Mat& mat);

	Mat PixelStatistics(Mat img, int t);

	void division_ostu(InputArray _src, int grid_x, int grid_y, Color type);

	bool remove_rivet(Mat& img);

	Rect rectEnlarge(const Rect& src, const int mat_width, const int mat_height);

	Rect rectEnlarge2(const Rect& src, const int mat_width, const int mat_height);

	bool SlideWindow(const Mat& image, Rect& inRect, Mat& newImage, Color plateType, float slideLengthRatio);

	int plateDetect(Mat src, std::vector<Plate>& resultVec, int type, bool showDetectArea, int img_index);

}