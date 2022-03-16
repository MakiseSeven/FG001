#pragma once
#include "opencv2/opencv.hpp"
#include "config.h"

using namespace cv;
using namespace std;

namespace Tsolpr {

    class CharsSegment {
    public:

        int charsSegmentUsingOSTU(Mat input, std::vector<Mat>& resultVec, std::vector<Mat>& grayChars, Color color = BLUE);

        bool verifyCharSizes(Mat r);

        Rect GetChineseRect(const Rect rectSpe);

        int FindChineseCharRect(const std::vector<Rect>& vecRect);

        int RebuildRect(const std::vector<Rect>& vecRect, std::vector<Rect>& outRect,
            int specIndex); 

        void makeCharCenter(Mat &in);

        const int DEFAULT_DEBUG = 1;
        
        const int CHAR_SIZE = 20;
        const int HORIZONTAL = 1;
        const int VERTICAL = 0;
        
        const int DEFAULT_LIUDING_SIZE = 7;
        const int DEFAULT_MAT_WIDTH = 136;
        const int DEFAULT_COLORTHRESHOLD = 150;
        
    private:

    };

}