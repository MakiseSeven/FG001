#include "char_segmentation.h"
#include <opencv2\imgproc\types_c.h>
#include"sundry_function.h"

int Tsolpr::CharsSegment::charsSegmentUsingOSTU(Mat input, std::vector<Mat>& resultVec, std::vector<Mat>& grayChars, Color color)
{
    if (!input.data) return 1;

    Color plateColor = color;
    Mat input_grey;
    cvtColor(input, input_grey, CV_BGR2GRAY);

    Mat img_threshold;
    img_threshold = input_grey.clone();
    division_ostu(img_threshold, 8, 2, plateColor);
    if (!remove_rivet(img_threshold)) return 2;

    Mat img_contours;
    img_threshold.copyTo(img_contours);
    vector<vector<Point> > contours;
    findContours(img_contours,
        contours,              
        CV_RETR_EXTERNAL,      
        CV_CHAIN_APPROX_NONE); 

    vector<vector<Point> >::iterator itc = contours.begin();
    vector<Rect> vecRect;
    while (itc != contours.end()) {
        Rect charRect = boundingRect(Mat(*itc));
        Mat signalCharRect(img_threshold, charRect);
        if (verifyCharSizes(signalCharRect))
            vecRect.push_back(charRect);
        ++itc;
    }

    if (vecRect.size() == 0) return 0x03;

    vector<Rect> sortedRect(vecRect);
    std::sort(sortedRect.begin(), sortedRect.end(),
        [](const Rect& r1, const Rect& r2) { return r1.x < r2.x; });

    size_t specIndex = 0;
    specIndex = FindChineseCharRect(sortedRect);

    Rect chineseRect;
    if (specIndex < sortedRect.size())
        chineseRect = GetChineseRect(sortedRect[specIndex]);
    else
        return 4;

    vector<Rect> newSortedRect;
    newSortedRect.push_back(chineseRect);
    RebuildRect(sortedRect, newSortedRect, specIndex);
    if (newSortedRect.size() == 0) return 5;

    for (size_t i = 0; i < newSortedRect.size(); i++) {
        Rect charRect = newSortedRect[i];
        Mat signalCharRect(input_grey, charRect);
        Mat newRoi;

        if (i == 0) {
            Rect biggerRect = rectEnlarge(charRect, input_grey.cols, input_grey.rows);
            Mat grayChar(input_grey, biggerRect);
            Mat grayChinese;
            grayChinese.create(kGrayCharHeight, kGrayCharWidth, CV_8UC1);
            resize(grayChar, grayChinese, grayChinese.size(), 0, 0, INTER_LINEAR);

            Mat newChineseRect;
            float slideLengthRatio = 0.1f;
            if (!SlideWindow(input_grey, biggerRect, newChineseRect, plateColor, slideLengthRatio)) {
                newChineseRect = grayChinese;
            }
            grayChars.push_back(newChineseRect);
        }
        else {
            switch (plateColor) {
            case BLUE:   threshold(signalCharRect, newRoi, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU); break;
            case YELLOW:   threshold(signalCharRect, newRoi, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU); break;
            case WHITE:  threshold(signalCharRect, newRoi, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV); break;
            default: threshold(signalCharRect, newRoi, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY); break;
            }
            makeCharCenter(newRoi);
            Rect fit_mr = rectEnlarge2(charRect, input_grey.cols, input_grey.rows);
            Mat grayChar(input_grey, fit_mr);
            grayChars.push_back(grayChar);
        }
        resultVec.push_back(newRoi);
        
    }

    return 0;
}

bool Tsolpr::CharsSegment::verifyCharSizes(Mat r)
{
    // Char sizes 45x90
    float aspect = 45.0f / 90.0f;
    float charAspect = (float)r.cols / (float)r.rows;
    float error = 0.7f;
    float minHeight = 10.f;
    float maxHeight = 36.f;
    // We have a different aspect ratio for number 1, and it can be ~0.2
    float minAspect = 0.05f;
    float maxAspect = aspect + aspect * error;
    // area of pixels
    int area = cv::countNonZero(r);
    // bb area
    int bbArea = r.cols * r.rows;
    //% of pixel in area
    int percPixels = area / bbArea;
    if (percPixels <= 1 && charAspect > minAspect && charAspect < maxAspect &&
        r.rows >= minHeight && r.rows < maxHeight)
        return true;
    else
        return false;
}

Rect Tsolpr::CharsSegment::GetChineseRect(const Rect rectSpe)
{
    int height = rectSpe.height;
    float biggerwidth = rectSpe.width * 1.15f;
    int x = rectSpe.x;
    int y = rectSpe.y;

    int newx = x - int(biggerwidth * 1.15);
    newx = newx > 0 ? newx : 0;

    Rect out(newx, y, int(biggerwidth), height);

    return out;
}

int Tsolpr::CharsSegment::FindChineseCharRect(const std::vector<Rect>& vecRect)
{
    vector<int> xpositions;
    int maxHeight = 0;
    int maxWidth = 0;

    for (size_t i = 0; i < vecRect.size(); i++) {
        xpositions.push_back(vecRect[i].x);

        if (vecRect[i].height > maxHeight) {
            maxHeight = vecRect[i].height;
        }
        if (vecRect[i].width > maxWidth) {
            maxWidth = vecRect[i].width;
        }
    }

    int specIndex = 0;
    for (size_t i = 0; i < vecRect.size(); i++) {
        Rect mr = vecRect[i];
        int midx = mr.x + mr.width / 2;

        // use prior knowledage to find the specific character
        // position in 1/7 and 2/7
        if ((mr.width > maxWidth * 0.6 || mr.height > maxHeight * 0.6) &&
            (midx < int(DEFAULT_MAT_WIDTH / kPlateMaxSymbolCount) * kSymbolIndex &&
                midx > int(DEFAULT_MAT_WIDTH / kPlateMaxSymbolCount) * (kSymbolIndex - 1))) {
            specIndex = i;
        }
    }

    return specIndex;
}

int Tsolpr::CharsSegment::RebuildRect(const std::vector<Rect>& vecRect, std::vector<Rect>& outRect, int specIndex)
{
    int count = 6;
    for (size_t i = specIndex; i < vecRect.size() && count; ++i, --count) {
        outRect.push_back(vecRect[i]);
    }

    return 0;
}

void Tsolpr::CharsSegment::makeCharCenter(Mat& in)
{
    // Remap image
    int h = in.rows;
    int w = in.cols;

    int charSize = CHAR_SIZE;

    Mat transformMat = Mat::eye(2, 3, CV_32F);
    int m = max(w, h);
    transformMat.at<float>(0, 2) = float(m / 2 - w / 2);
    transformMat.at<float>(1, 2) = float(m / 2 - h / 2);

    Mat warpImage(m, m, in.type());
    warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    resize(warpImage, in, Size(charSize, charSize));
}
