#include"opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
#include"sundry_function.h"
using namespace cv;

namespace Tsolpr {
    Mat getHistogram(Mat in) {

        Mat RowFeature = PixelStatistics(in, 0);
        Mat ColFeature = PixelStatistics(in, 1);

        int numCols = RowFeature.cols + ColFeature.cols;

        Mat out = Mat::zeros(1, numCols, CV_32F);

        int j = 0;
        for (int i = 0; i < RowFeature.cols; i++) {
            out.at<float>(j) = RowFeature.at<float>(i);
            j++;
        }
        for (int i = 0; i < ColFeature.cols; i++) {
            out.at<float>(j) = ColFeature.at<float>(i);
            j++;
        }

        return out;
    }

    void getHistogramFeatures(const Mat& image, Mat& features) {
        Mat ImageGray;

        cvtColor(image, ImageGray, CV_RGB2GRAY);

        Mat ImageThreshold;
        threshold(ImageGray, ImageThreshold, 0, 255, CV_THRESH_OTSU);


        features = getHistogram(ImageThreshold);
    }

    void getColorFeatures(const Mat& src, Mat& features) {
        Mat src_hsv;

        cvtColor(src, src_hsv, CV_BGR2HSV);
        int channels = src_hsv.channels();
        int nRows = src_hsv.rows;

        int nCols = src_hsv.cols * channels;
        if (src_hsv.isContinuous()) {
            nCols *= nRows;
            nRows = 1;
        }

        const int all = 180;
        int a[all] = { 0 };
        uchar* p;
        for (int i = 0; i < nRows; ++i) {
            p = src_hsv.ptr<uchar>(i);
            for (int j = 0; j < nCols; j += 3) {
                int H = int(p[j]);      // 0-180
                if (H > all - 1) H = all - 1;
                if (H < 0) H = 0;
                a[H]++;
            }
        }

        Mat out = Mat::zeros(1, all, CV_32F);
        for (int j = 0; j < all; j++) {
            out.at<float>(j) = (float)a[j];
        }

        double min, max;
        minMaxLoc(out, &min, &max);

        if (max > 0)
            out.convertTo(out, -1, 1.0f / max, 0);

        features = out;
    }

    void FeatureCreate(const Mat& image, Mat& features) {
        // TODO
        Mat feature1, feature2;
        getHistogramFeatures(image, feature1);
        getColorFeatures(image, feature2);
        hconcat(feature1.reshape(1, 1), feature2.reshape(1, 1), features);
    }

    Mat CharFeatureCreate(const Mat& in, int sizeData) {
        const int VERTICAL = 0;
        const int HORIZONTAL = 1;

        Mat lowData;
        resize(in, lowData, Size(sizeData, sizeData));

        Mat vhist = PixelStatistics(lowData, VERTICAL);
        Mat hhist = PixelStatistics(lowData, HORIZONTAL);

        int numCols = vhist.cols + hhist.cols;

        Mat out = Mat::zeros(1, numCols, CV_32F);

        int j = 0;
        for (int i = 0; i < vhist.cols; i++) {
            out.at<float>(j) = vhist.at<float>(i);
            j++;
        }
        for (int i = 0; i < hhist.cols; i++) {
            out.at<float>(j) = hhist.at<float>(i);
            j++;
        }

        return out;
    }

    void ExtractCharFeature(const Mat& grayChar, Mat& features)
    {
        bool useResize = false;
        bool useConvert = true;
        bool useMean = true;
        bool useLBP = false;

        Mat char_mat;
        char_mat = grayChar;

        Mat float_img;
        float scale = 1.f / 255;
        char_mat.convertTo(float_img, CV_32FC1, scale, 0);

        Mat mean_img;
        float_img -= mean(float_img);
        mean_img = float_img;

        Mat feautreImg;
        feautreImg = mean_img.reshape(1, 1);

        Mat binaryChar;
        threshold(grayChar, binaryChar, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

        Mat projectFeature = CharFeatureCreate(binaryChar, 32);

        hconcat(feautreImg.reshape(1, 1), projectFeature.reshape(1, 1), features);
    }

    Rect CenterRect(Mat& in) {
        Rect _rect;

        int top = 0;
        int bottom = in.rows - 1;

        // find the center rect

        for (int i = 0; i < in.rows; ++i) {
            bool bFind = false;
            for (int j = 0; j < in.cols; ++j) {
                if (in.data[i * in.step[0] + j] > 20) {
                    top = i;
                    bFind = true;
                    break;
                }
            }
            if (bFind) {
                break;
            }

        }
        for (int i = in.rows - 1;
            i >= 0;
            --i) {
            bool bFind = false;
            for (int j = 0; j < in.cols; ++j) {
                if (in.data[i * in.step[0] + j] > 20) {
                    bottom = i;
                    bFind = true;
                    break;
                }
            }
            if (bFind) {
                break;
            }

        }


        int left = 0;
        int right = in.cols - 1;
        for (int j = 0; j < in.cols; ++j) {
            bool bFind = false;
            for (int i = 0; i < in.rows; ++i) {
                if (in.data[i * in.step[0] + j] > 20) {
                    left = j;
                    bFind = true;
                    break;
                }
            }
            if (bFind) {
                break;
            }

        }
        for (int j = in.cols - 1;
            j >= 0;
            --j) {
            bool bFind = false;
            for (int i = 0; i < in.rows; ++i) {
                if (in.data[i * in.step[0] + j] > 20) {
                    right = j;
                    bFind = true;

                    break;
                }
            }
            if (bFind) {
                break;
            }
        }

        _rect.x = left;
        _rect.y = top;
        _rect.width = right - left + 1;
        _rect.height = bottom - top + 1;

        return _rect;
    }

    Mat CutTheRect(Mat& in, Rect& rect) {
        int size = in.cols;  // (rect.width>rect.height)?rect.width:rect.height;
        Mat dstMat(size, size, CV_8UC1);
        dstMat.setTo(Scalar(0, 0, 0));

        int x = (int)floor((float)(size - rect.width) / 2.0f);
        int y = (int)floor((float)(size - rect.height) / 2.0f);

        for (int i = 0; i < rect.height; ++i) {

            for (int j = 0; j < rect.width; ++j) {
                dstMat.data[dstMat.step[0] * (i + y) + j + x] =
                    in.data[in.step[0] * (i + rect.y) + j + rect.x];
            }
        }
        //
        return dstMat;
    }

    Mat AlphaCharFeatures(Mat in, int sizeData) {
        const int VERTICAL = 0;
        const int HORIZONTAL = 1;

        Rect _rect = CenterRect(in);
        Mat tmpIn = CutTheRect(in, _rect);

        Mat lowData;
        resize(tmpIn, lowData, Size(sizeData, sizeData));

        Mat vhist = PixelStatistics(lowData, VERTICAL);
        Mat hhist = PixelStatistics(lowData, HORIZONTAL);

        int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;

        Mat out = Mat::zeros(1, numCols, CV_32F);

        int j = 0;
        for (int i = 0; i < vhist.cols; i++) {
            out.at<float>(j) = vhist.at<float>(i);
            j++;
        }
        for (int i = 0; i < hhist.cols; i++) {
            out.at<float>(j) = hhist.at<float>(i);
            j++;
        }
        for (int x = 0; x < lowData.cols; x++) {
            for (int y = 0; y < lowData.rows; y++) {
                out.at<float>(j) += (float)lowData.at <unsigned char>(x, y);
                j++;
            }
        }

        return out;

    }
}