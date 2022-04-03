#pragma once
#include<iostream>
#include<vector>
#include"config.h"
#include"opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
#include"basic_plate.hpp"
#include"utils.h"
#include"char_classify.h"
#include"plate_judge.h"
#include"Plate_locate.h"

using namespace cv;

namespace Tsolpr {

    bool verifySizes(RotatedRect mr) {
        float error = 0.9;
        float aspect = 3.75;

        int min = 272; 
        int max = 54400;

        float rmin = aspect - aspect * error;
        float rmax = aspect + aspect * error;

        float area = mr.size.height * mr.size.width;
        float r = (float)mr.size.width / (float)mr.size.height;
        if (r < 1) r = (float)mr.size.height / (float)mr.size.width;

        if ((area < min || area > max) || (r < rmin || r > rmax))
            return false;
        else
            return true;
    }

    Mat ColoeMatch(const Mat& src, Mat& out, const Color color) {


        const float max_sv = 255;
        const float min_sv = 64;
        const float minabs_sv = 95;

        const int min_blue = 100;  
        const int max_blue = 140;  

        const int min_yellow = 15; 
        const int max_yellow = 40; 

        const int min_white = 0;   
        const int max_white = 30;  

        Mat src_hsv;

        cvtColor(src, src_hsv, CV_BGR2HSV);

        std::vector<cv::Mat> hsvSplit;
        split(src_hsv, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);
        merge(hsvSplit, src_hsv);

        int min_h = 0;
        int max_h = 0;
        switch (color) {
        case BLUE:
            min_h = min_blue;
            max_h = max_blue;
            break;
        case YELLOW:
            min_h = min_yellow;
            max_h = max_yellow;
            break;
        case WHITE:
            min_h = min_white;
            max_h = max_white;
            break;
        default:
            break;
        }

        float diff_h = float((max_h - min_h) / 2);
        float avg_h = (min_h + max_h) / 2;

        int channels = src_hsv.channels();
        int nRows = src_hsv.rows;
        int nCols = src_hsv.cols * channels;
        if (src_hsv.isContinuous()) {
            nCols *= nRows;
            nRows = 1;
        }

        int i, j;
        uchar* p;
        for (i = 0; i < nRows; ++i) {
            p = src_hsv.ptr<uchar>(i);
            for (j = 0; j < nCols; j += 3) {
                int H = int(p[j]);      // 0-180
                int S = int(p[j + 1]);  // 0-255
                int V = int(p[j + 2]);  // 0-255

                bool colorMatched = false;

                if (H > min_h&& H < max_h) {
                    if ((S > minabs_sv&& S < max_sv) && (V > minabs_sv&& V < max_sv))
                        colorMatched = true;
                }

                if (colorMatched == true) {
                    p[j] = 0;
                    p[j + 1] = 0;
                    p[j + 2] = 255;
                }
                else {
                    p[j] = 0;
                    p[j + 1] = 0;
                    p[j + 2] = 0;
                }
            }
        }
        Mat src_grey;
        std::vector<cv::Mat> hsvSplit_2;
        split(src_hsv, hsvSplit_2);
        src_grey = hsvSplit_2[2];

        out = src_grey;

        return src_grey;
    }

    bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv,float& percent) {

        const float thresh = 0.45f;

        Mat src_gray;
        ColoeMatch(src, src_gray, r);

        percent =
            float(countNonZero(src_gray)) / float(src_gray.rows * src_gray.cols);
        // cout << "percent:" << percent << endl;

        if (percent > thresh)
            return true;
        else
            return false;
    }

    void TopBottomFound(Mat mask, int& top, int& bottom) {
        const int x = 7;

        for (int i = 0; i < mask.rows / 2; i++) {
            int whitePixel = 0;
            int boundCount = 0;
            for (int j = 0; j < mask.cols - 1; j++) {
                if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) boundCount++;

                if ((int)mask.at<uchar>(i, j) == 255) {
                    whitePixel++;
                }
            }
            if ((boundCount < x && whitePixel * 1.0 / mask.cols < 0.2) ||
                whitePixel < 4) {
                top = i;

            }
        }
        top -= 1;
        if (top < 0) {
            top = 0;
        }

        for (int i = mask.rows - 1; i >= mask.rows / 2; i--) {
            int boundCount = 0;
            int whitePixel = 0;
            for (int j = 0; j < mask.cols - 1; j++) {
                if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) boundCount++;
                if (mask.at<uchar>(i, j) == 255) {
                    whitePixel++;
                }
            }
            if ((boundCount < x && whitePixel * 1.0 / mask.cols < 0.2) ||
                whitePixel < 4) {
                bottom = i;
            }
        }
        bottom += 1;
        if (bottom >= mask.rows) {
            bottom = mask.rows - 1;
        }

        if (top >= bottom) {
            top = 0;
            bottom = mask.rows - 1;
        }
    }

    bool LeftRightFound(Mat& src, int& left, int& right) {

        float span = src.rows * 0.2f;

        for (int i = 0; i < src.cols - span - 1; i += 3) {
            int whiteCount = 0;
            for (int k = 0; k < src.rows; k++) {
                for (int l = i; l < i + span; l++) {
                    if (src.data[k * src.step[0] + l] == 255) {
                        whiteCount++;
                    }
                }
            }
            if (whiteCount * 1.0 / (span * src.rows) > 0.15) {
                left = i;
                break;
            }
        }

        for (int i = src.cols - 1; i > span; i -= 2) {
            int whiteCount = 0;
            for (int k = 0; k < src.rows; k++) {
                for (int l = i; l > i - span; l--) {
                    if (src.data[k * src.step[0] + l] == 255) {
                        whiteCount++;
                    }
                }
            }

            if (whiteCount * 1.0 / (span * src.rows) > 0.06) {
                right = i;
                if (right + 5 < src.cols) {
                    right = right + 5;
                }
                else {
                    right = src.cols - 1;
                }

                break;
            }
        }
        if (left < right) {
            return true;
        }
        return false;
    }

    Color getPlateType(const Mat& src, const bool adaptive_minsv) {
        float max_percent = 0;
        Color max_color = UNKNOWN;

        float blue_percent = 0;
        float yellow_percent = 0;
        float white_percent = 0;

        if (plateColorJudge(src, BLUE, adaptive_minsv, blue_percent) == true) {
            return BLUE;
        }
        else if (plateColorJudge(src, YELLOW, adaptive_minsv, yellow_percent) ==
            true) {
            return YELLOW;
        }
        else if (plateColorJudge(src, WHITE, adaptive_minsv, white_percent) ==
            true) {
            // cout << "WHITE" << endl;
            return WHITE;
        }
        else {
            //std::cout << "OTHER" << std::endl;

            /*max_percent = blue_percent > yellow_percent ? blue_percent : yellow_percent;
            max_color = blue_percent > yellow_percent ? BLUE : YELLOW;
            max_color = max_percent > white_percent ? max_color : WHITE;*/

            // always return blue
            return BLUE;
        }
    }

    void deleteNotArea(Mat& inMat, Color color = UNKNOWN) {
        Mat inMat_grey;
        cvtColor(inMat, inMat_grey, CV_BGR2GRAY);

        int w = inMat.cols;
        int h = inMat.rows;

        Mat tmpMat = inMat(Rect_<double>(w * 0.15, h * 0.1, w * 0.7, h * 0.7));

        Color plateType;
        if (UNKNOWN == color) {
            plateType = getPlateType(tmpMat, true);
        }
        else {
            plateType = color;
        }

        Mat img_threshold;

        threshold(inMat_grey, img_threshold, 0, 255, THRESH_BINARY | THRESH_OTSU);

        int left = 0;
        int right = 0;
        int top = 0;
        int bottom = img_threshold.rows - 1;
        TopBottomFound(img_threshold, top, bottom);
        img_threshold = img_threshold(Rect(0, top, inMat.cols, bottom - top));
        if (LeftRightFound(img_threshold, left, right)) {
            inMat = inMat(Rect(left, top, inMat.cols-left, bottom - top));
       }

    }

    Mat Resize(const Mat& result, float& scale) {
        const int RESULTWIDTH = kShowWindowWidth;   // 640 930
        const int RESULTHEIGHT = kShowWindowHeight;  // 540 710

        Mat img_window;
        img_window.create(RESULTHEIGHT, RESULTWIDTH, CV_8UC3);

        int nRows = result.rows;
        int nCols = result.cols;

        Mat result_resize;
        if (nCols <= img_window.cols && nRows <= img_window.rows) {
            result_resize = result;
        }
        else if (nCols > img_window.cols&& nRows <= img_window.rows) {
            scale = float(img_window.cols) / float(nCols);
            resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

        }
        else if (nCols <= img_window.cols && nRows > img_window.rows) {
            scale = float(img_window.rows) / float(nRows);
            resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

        }
        else if (nCols > img_window.cols&& nRows > img_window.rows) {
            float scale1 = float(img_window.cols) / float(nCols);
            float scale2 = float(img_window.rows) / float(nRows);
            scale = scale1 < scale2 ? scale1 : scale2;
            resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);
        }
        else {
            result_resize = result;
        }
        return result_resize;
    }

    RotatedRect rectResize(const RotatedRect& rr, const float scale_ratio) {
        float width = rr.size.width * scale_ratio;
        float height = rr.size.height * scale_ratio;
        float x = rr.center.x * scale_ratio;
        float y = rr.center.y * scale_ratio;
        RotatedRect mserRect(Point2f(x, y), Size2f(width, height), rr.angle);

        return mserRect;
    }

    Rect interRect(const Rect& a, const Rect& b) {
        Rect c;
        int x1 = a.x > b.x ? a.x : b.x;
        int y1 = a.y > b.y ? a.y : b.y;
        c.width = (a.x + a.width < b.x + b.width ? a.x + a.width : b.x + b.width) - x1;
        c.height = (a.y + a.height < b.y + b.height ? a.y + a.height : b.y + b.height) - y1;
        c.x = x1;
        c.y = y1;
        if (c.width <= 0 || c.height <= 0)
            c = Rect();
        return c;
    }

    Rect outRect(const Rect& a, const Rect& b) {
        Rect c;
        int x1 = a.x < b.x ? a.x : b.x;
        int y1 = a.y < b.y ? a.y : b.y;
        c.width = (a.x + a.width > b.x + b.width ? a.x + a.width : b.x + b.width) - x1;
        c.height = (a.y + a.height > b.y + b.height ? a.y + a.height : b.y + b.height) - y1;
        c.x = x1;
        c.y = y1;
        return c;
    }

    float computeIOU(const Rect& rect1, const Rect& rect2) {

        Rect inter = interRect(rect1, rect2);
        Rect out = outRect(rect1, rect2);

        float iou = (float)inter.area() / (float)out.area();

        return iou;
    }

    void NMS(std::vector<Plate>& inVec, std::vector<Plate>& resultVec, double overlap) {
        std::sort(inVec.begin(), inVec.end());
        std::vector<Plate>::iterator it = inVec.begin();
        for (; it != inVec.end(); ++it) {
            Plate plateSrc = *it;
            Rect rectSrc = plateSrc.getPlatePos().boundingRect();
            std::vector<Plate>::iterator itc = it + 1;
            for (; itc != inVec.end();) {
                Plate plateComp = *itc;
                Rect rectComp = plateComp.getPlatePos().boundingRect();
                float iou = computeIOU(rectSrc, rectComp);
                if (iou > overlap) {
                    itc = inVec.erase(itc);
                }
                else {
                    ++itc;
                }
            }
        }
        resultVec = inVec;
    }

    void NMS(std::vector<Character>& inVec, double overlap) {
        std::sort(inVec.begin(), inVec.end());

        std::vector<Character>::iterator it = inVec.begin();
        for (; it != inVec.end(); ++it) {
            Character charSrc = *it;
            Rect rectSrc = charSrc.getCharacterRect();

            std::vector<Character>::iterator itc = it + 1;

            for (; itc != inVec.end();) {
                Character charComp = *itc;
                Rect rectComp = charComp.getCharacterRect();

                float iou = computeIOU(rectSrc, rectComp);

                if (iou > overlap) {
                    itc = inVec.erase(itc);
                }
                else {
                    ++itc;
                }
            }
        }
    }

    bool RectRevise(const RotatedRect& InRect_b, const Mat& src, Rect_<float>& Rect_Revise) {
        Rect_<float> BoundRect = InRect_b.boundingRect();

        float tl_x = BoundRect.x > 0 ? BoundRect.x : 0;
        float tl_y = BoundRect.y > 0 ? BoundRect.y : 0;

        float br_x = BoundRect.x + BoundRect.width < src.cols
            ? BoundRect.x + BoundRect.width - 1
            : src.cols - 1;
        float br_y = BoundRect.y + BoundRect.height < src.rows
            ? BoundRect.y + BoundRect.height - 1
            : src.rows - 1;

        float resive_width = br_x - tl_x;
        float resive_height = br_y - tl_y;

        if (resive_width <= 0 || resive_height <= 0) return false;

        Rect_Revise = Rect_<float>(tl_x, tl_y, resive_width, resive_height);

        return true;
    }

    float BlackPixelStatistic(Mat& mat) {
        float Count = 0.0;
        if (mat.rows > 1) {
            for (int i = 0; i < mat.rows; ++i) {
                if (mat.data[i * mat.step[0]] > 0) {
                    Count += 1.0;
                }
            }
            return Count;

        }
        else {
            for (int i = 0; i < mat.cols; ++i) {
                if (mat.data[i] > 0) {
                    Count += 1.0;
                }
            }

            return Count;
        }
    }

    Mat PixelStatistics(Mat img, int t) {
        int all = (t) ? img.rows : img.cols;
        Mat out = Mat::zeros(1, all, CV_32F);
        for (int j = 0; j < all; j++) {
            Mat data = (t) ? img.row(j) : img.col(j);

            out.at<float>(j) = BlackPixelStatistic(data);
        }

        // Normalize histogram
        double min, max;
        minMaxLoc(out, &min, &max);

        if (max > 0)
            out.convertTo(out, -1, 1.0f / max, 0);

        return out;
    }

    void division_ostu(InputArray _src, int grid_x, int grid_y, Color type) {
        Mat src = _src.getMat();

        int width = src.cols / grid_x;
        int height = src.rows / grid_y;

        // iterate through grid
        for (int i = 0; i < grid_y; i++) {
            for (int j = 0; j < grid_x; j++) {
                Mat src_cell = Mat(src, Range(i * height, (i + 1) * height), Range(j * width, (j + 1) * width));
                if (type == BLUE) {
                    cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);
                }
                else if (type == YELLOW) {
                    cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY_INV);
                }
                else if (type == WHITE) {
                    cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY_INV);
                }
                else {
                    cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);
                }
            }
        }
    }

    bool remove_rivet(Mat& img) {
        std::vector<float> fJump;
        int whitePixel = 0;
        const int x = 7;
        Mat jump = Mat::zeros(1, img.rows, CV_32F);
        for (int i = 0; i < img.rows; i++) {
            int jumpCount = 0;
            for (int j = 0; j < img.cols - 1; j++) {
                if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

                if (img.at<uchar>(i, j) == 255) {
                    whitePixel++;
                }
            }

            jump.at<float>(i) = (float)jumpCount;
        }

        int iCount = 0;
        for (int i = 0; i < img.rows; i++) {
            fJump.push_back(jump.at<float>(i));
            if (jump.at<float>(i) >= 16 && jump.at<float>(i) <= 45) {

                iCount++;
            }
        }
        if (iCount * 1.0 / img.rows <= 0.40) {
            return false;
        }

        if (whitePixel * 1.0 / (img.rows * img.cols) < 0.15 ||
            whitePixel * 1.0 / (img.rows * img.cols) > 0.50) {
            return false;
        }

        for (int i = 0; i < img.rows; i++) {
            if (jump.at<float>(i) <= x) {
                for (int j = 0; j < img.cols; j++) {
                    img.at<char>(i, j) = 0;
                }
            }
        }
        return true;
    }

    Rect rectEnlarge(const Rect& src, const int mat_width, const int mat_height) {
        float w = (float)src.width;
        float h = (float)src.height;
        float new_w = w * 1.2f;
        float new_h = h * 1.05f;

        Rect_<float> boudRect;
        boudRect.x = (float)src.x - w * 0.1f;
        boudRect.y = (float)src.y - h * 0.025f;
        boudRect.width = new_w;
        boudRect.height = new_h;

        float tl_x = boudRect.x > 0 ? boudRect.x : 0;
        float tl_y = boudRect.y > 0 ? boudRect.y : 0;

        float br_x = boudRect.x + boudRect.width - 1 <= mat_width - 1
            ? boudRect.x + boudRect.width - 1
            : mat_width - 1;
        float br_y = boudRect.y + boudRect.height - 1 < mat_height - 1
            ? boudRect.y + boudRect.height - 1
            : mat_height - 1;

        float roi_width = br_x - tl_x + 1;
        float roi_height = br_y - tl_y + 1;

        Rect dst(0, 0, 0, 0);
        if (roi_width <= 0 || roi_height <= 0)
            return src;

        dst = Rect_<float>(tl_x, tl_y, roi_width, roi_height);
        return dst;

    }

    Rect rectEnlarge2(const Rect& src, const int mat_width, const int mat_height) {
        float w = (float)src.width;
        float h = (float)src.height;
        float new_w = h * 0.5f;
        float new_h = h * 1.05f;

        if (new_w <= w || new_h <= h) {
            return src;
        }

        float ex_w = (new_w - w) * 0.5f;
        float ex_h = (new_h - h) * 0.5f;

        Rect_<float> boudRect;
        boudRect.x = (float)src.x - ex_w;
        boudRect.y = (float)src.y - ex_h;
        boudRect.width = new_w;
        boudRect.height = new_h;

        float tl_x = boudRect.x > 0 ? boudRect.x : 0;
        float tl_y = boudRect.y > 0 ? boudRect.y : 0;

        float br_x = boudRect.x + boudRect.width - 1 <= mat_width - 1
            ? boudRect.x + boudRect.width - 1
            : mat_width - 1;
        float br_y = boudRect.y + boudRect.height - 1 < mat_height - 1
            ? boudRect.y + boudRect.height - 1
            : mat_height - 1;

        float roi_width = br_x - tl_x + 1;
        float roi_height = br_y - tl_y + 1;

        Rect dst(0, 0, 0, 0);
        if (roi_width <= 2 || roi_height <= 2)
            return src;

        //! a new rect not out the range of mat
        dst = Rect_<float>(tl_x, tl_y, roi_width - 1, roi_height - 1);
        return dst;

    }

    bool SlideWindow(const Mat& image, Rect& inRect, Mat& newImage, Color plateType, float slideLengthRatio) {
        std::vector<Character> charCandidateVec;

        Rect maxrect = inRect;
        Point tlPoint = inRect.tl();

        bool isChinese = true;
        int slideLength = int(slideLengthRatio * maxrect.width);
        int slideStep = 1;
        int fromX = 0;
        fromX = tlPoint.x;

        for (int slideX = -slideLength; slideX < slideLength; slideX += slideStep) {
            float x_slide = 0;
            x_slide = float(fromX + slideX);

            float y_slide = (float)tlPoint.y;

            int chineseWidth = int(maxrect.width);
            int chineseHeight = int(maxrect.height);

            Rect rect(Point2f(x_slide, y_slide), Size(chineseWidth, chineseHeight));

            if (rect.tl().x < 0 || rect.tl().y < 0 || rect.br().x >= image.cols || rect.br().y >= image.rows)
                continue;

            Mat auxRect = image(rect);
            Mat grayChinese;
            grayChinese.create(kGrayCharHeight, kGrayCharWidth, CV_8UC1);
            resize(auxRect, grayChinese, grayChinese.size(), 0, 0, INTER_LINEAR);

            Character charCandidateOstu;
            charCandidateOstu.setCharacterRect(rect);
            charCandidateOstu.setCharacterMat(grayChinese);
            charCandidateOstu.setIsChinese(isChinese);
            charCandidateVec.push_back(charCandidateOstu);
        }

        CharClassify::instance()->ClassifyChineseGray(charCandidateVec);

        double overlapThresh = 0.1;
        NMS(charCandidateVec, overlapThresh);

        if (charCandidateVec.size() >= 1) {
            std::sort(charCandidateVec.begin(), charCandidateVec.end(),
                [](const Character& r1, const Character& r2) {
                return r1.getCharacterScore() > r2.getCharacterScore();
            });

            newImage = charCandidateVec.at(0).getCharacterMat();
            inRect = charCandidateVec.at(0).getCharacterRect();
            return true;
        }
        return false;
    }

    int plateDetect(Mat src, std::vector<Plate>& resultVec, int type, bool showDetectArea, int img_index)
    {
        std::vector<Plate> color_Plates;
        color_Plates.reserve(16);
        std::vector<Plate> resultPlate;
        resultPlate.reserve(64);

        PlateLocate::instance()->ColorLocate(src, color_Plates, img_index);

        for (auto plate : color_Plates) {
            plate.setPlateLocateType(COLOR);
            resultPlate.push_back(plate);
        }

        PlateJudge::point()->plateJudgeUsingNMS(resultPlate, resultVec, 4);

        return 0;
    }

}