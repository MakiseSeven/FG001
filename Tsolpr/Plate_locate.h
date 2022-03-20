#pragma once
#include"basic_plate.hpp"

using namespace std;

namespace Tsolpr {

    class PlateLocate {
    public:
        PlateLocate() {};

        static PlateLocate* instance();

        int deskew(const Mat& src, const Mat& src_b,std::vector<RotatedRect>& inRects, std::vector<Plate>& outPlates, Color color = UNKNOWN);

        bool rotation(Mat& in, Mat& out, const Size rect_size, const Point2f center,
            const double angle);

        int ColorLocate(Mat src, std::vector<Plate>& PlateVec, int count = 0);

        int ColorSearch(const Mat& src, const Color r, Mat& out,
            std::vector<RotatedRect>& outRects);

        static const int DEFAULT_GAUSSIANBLUR_SIZE = 5;
        static const int SOBEL_SCALE = 1;
        static const int SOBEL_DELTA = 0;
        static const int SOBEL_DDEPTH = CV_16S;
        static const int SOBEL_X_WEIGHT = 1;
        static const int SOBEL_Y_WEIGHT = 0;
        static const int DEFAULT_MORPH_SIZE_WIDTH = 17; 
        static const int DEFAULT_MORPH_SIZE_HEIGHT = 3; 


        static const int WIDTH = 136;
        static const int HEIGHT = 36;
        static const int TYPE = CV_8UC3;


        static const int DEFAULT_VERIFY_MIN = 1; 
        static const int DEFAULT_VERIFY_MAX = 24;

        static const int DEFAULT_ANGLE = 60;  

    protected:
        static PlateLocate* instance_;
    };

}