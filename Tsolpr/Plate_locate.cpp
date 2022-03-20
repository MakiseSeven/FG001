#include "Plate_locate.h"
#include"config.h"
#include <opencv2/imgproc/types_c.h>
#include"sundry_function.h"

//sobellocate had be hidden
namespace Tsolpr {

    PlateLocate* PlateLocate::instance_ = nullptr;

    //int Tsolpr::PlateLocate::sobelOper(const Mat& in, Mat& out, int blurSize, int morphW, int morphH)
    //{
    //    Mat mat_blur;
    //    mat_blur = in.clone();
    //    GaussianBlur(in, mat_blur, Size(blurSize, blurSize), 0, 0, BORDER_DEFAULT);
    //
    //    Mat mat_gray;
    //    if (mat_blur.channels() == 3)
    //        cvtColor(mat_blur, mat_gray, CV_RGB2GRAY);
    //    else
    //        mat_gray = mat_blur;
    //
    //    int scale = SOBEL_SCALE;
    //    int delta = SOBEL_DELTA;
    //    int ddepth = SOBEL_DDEPTH;
    //
    //    Mat grad_x, grad_y;
    //    Mat abs_grad_x, abs_grad_y;
    //
    //
    //    Sobel(mat_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    //    convertScaleAbs(grad_x, abs_grad_x);
    //
    //    Mat grad;
    //    addWeighted(abs_grad_x, SOBEL_X_WEIGHT, 0, 0, 0, grad);
    //
    //    Mat mat_threshold;
    //    double otsu_thresh_val =
    //        threshold(grad, mat_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
    //
    //
    //    Mat element = getStructuringElement(MORPH_RECT, Size(morphW, morphH));
    //    morphologyEx(mat_threshold, mat_threshold, MORPH_CLOSE, element);
    //
    //    out = mat_threshold;
    //
    //    return 0;
    //}

    //nt Tsolpr::PlateLocate::sobelFrtSearch(const Mat& src, std::vector<Rect_<float>>& outRects){
    // Mat src_threshold;
    // sobelOper(src, src_threshold, m_GaussianBlurSize, m_MorphSizeWidth,
    //     m_MorphSizeHeight);
    //
    // vector<vector<Point>> contours;
    // findContours(src_threshold,
    //     contours,               // a vector of contours
    //     CV_RETR_EXTERNAL,
    //     CV_CHAIN_APPROX_NONE);  // all pixels of each contours
    //
    // vector<vector<Point>>::iterator itc = contours.begin();
    //
    // vector<RotatedRect> first_rects;
    //
    // while (itc != contours.end()) {
    //     RotatedRect mr = minAreaRect(Mat(*itc));
    //
    //
    //     if (verifySizes(mr)) {
    //         first_rects.push_back(mr);
    //     }
    //
    //     ++itc;
    // }
    // for (size_t i = 0; i < first_rects.size(); i++) {
    //     RotatedRect InRect_b = first_rects[i];
    //
    //     Rect_<float> Rect_Revise;
    //     if (!calcSafeRect(InRect_b, src, Rect_Revise)) continue;
    //
    //     outRects.push_back(Rect_Revise);
    // }
    // return 0;
    //}

    //int Tsolpr::PlateLocate::plateSobelLocate(Mat src, std::vector<Plate>& candPlates, int index)
    //{
    //    vector<RotatedRect> rects_sobel_all;
    //    rects_sobel_all.reserve(256);
    //
    //    vector<Plate> plates;
    //    plates.reserve(32);
    //
    //    vector<Rect_<float>> bound_rects;
    //    bound_rects.reserve(256);
    //
    //    sobelFrtSearch(src, bound_rects);
    //
    //    vector<Rect_<float>> bound_rects_part;
    //    bound_rects_part.reserve(256);
    //
    //    // enlarge area 
    //    for (size_t i = 0; i < bound_rects.size(); i++) {
    //        float fRatio = bound_rects[i].width * 1.0f / bound_rects[i].height;
    //        if (fRatio < 3.0 && fRatio > 1.0 && bound_rects[i].height < 120) {
    //            Rect_<float> itemRect = bound_rects[i];
    //
    //            itemRect.x = itemRect.x - itemRect.height * (4 - fRatio);
    //            if (itemRect.x < 0) {
    //                itemRect.x = 0;
    //            }
    //            itemRect.width = itemRect.width + itemRect.height * 2 * (4 - fRatio);
    //            if (itemRect.width + itemRect.x >= src.cols) {
    //                itemRect.width = src.cols - itemRect.x;
    //            }
    //
    //            itemRect.y = itemRect.y - itemRect.height * 0.08f;
    //            itemRect.height = itemRect.height * 1.16f;
    //
    //            bound_rects_part.push_back(itemRect);
    //        }
    //    }

    //    // second processing to split one
    //#pragma omp parallel for
    //    for (int i = 0; i < (int)bound_rects_part.size(); i++) {
    //        Rect_<float> bound_rect = bound_rects_part[i];
    //        Point2f refpoint(bound_rect.x, bound_rect.y);
    //
    //        float x = bound_rect.x > 0 ? bound_rect.x : 0;
    //        float y = bound_rect.y > 0 ? bound_rect.y : 0;
    //
    //        float width =
    //            x + bound_rect.width < src.cols ? bound_rect.width : src.cols - x;
    //        float height =
    //            y + bound_rect.height < src.rows ? bound_rect.height : src.rows - y;
    //
    //        Rect_<float> safe_bound_rect(x, y, width, height);
    //        Mat bound_mat = src(safe_bound_rect);
    //
    //        vector<RotatedRect> rects_sobel;
    //        rects_sobel.reserve(128);
    //        sobelSecSearchPart(bound_mat, refpoint, rects_sobel);
    //
    //#pragma omp critical
    //        {
    //            rects_sobel_all.insert(rects_sobel_all.end(), rects_sobel.begin(), rects_sobel.end());
    //        }
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 0; i < (int)bound_rects.size(); i++) {
    //        Rect_<float> bound_rect = bound_rects[i];
    //        Point2f refpoint(bound_rect.x, bound_rect.y);
    //
    //        float x = bound_rect.x > 0 ? bound_rect.x : 0;
    //        float y = bound_rect.y > 0 ? bound_rect.y : 0;
    //
    //        float width =
    //            x + bound_rect.width < src.cols ? bound_rect.width : src.cols - x;
    //        float height =
    //            y + bound_rect.height < src.rows ? bound_rect.height : src.rows - y;
    //
    //        Rect_<float> safe_bound_rect(x, y, width, height);
    //        Mat bound_mat = src(safe_bound_rect);
    //
    //        vector<RotatedRect> rects_sobel;
    //        rects_sobel.reserve(128);
    //        sobelSecSearch(bound_mat, refpoint, rects_sobel);
    //
    //#pragma omp critical
    //        {
    //            rects_sobel_all.insert(rects_sobel_all.end(), rects_sobel.begin(), rects_sobel.end());
    //        }
    //    }
    //
    //    Mat src_b;
    //    sobelOper(src, src_b, 3, 10, 3);
    //
    //    deskew(src, src_b, rects_sobel_all, plates);
    //
    //    //for (size_t i = 0; i < plates.size(); i++) 
    //    //  candPlates.push_back(plates[i]);
    //
    //    candPlates.insert(candPlates.end(), plates.begin(), plates.end());
    //
    //    return 0;
    //}

    int Tsolpr::PlateLocate::ColorSearch(const Mat& src, const Color color, Mat& out, std::vector<RotatedRect>& outRects)
    {
        Mat match_grey;

        const int color_morph_width = 10;
        const int color_morph_height = 2;

        ColoeMatch(src, match_grey, color);

        Mat src_threshold;
        threshold(match_grey, src_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

        Mat element = getStructuringElement(
            MORPH_RECT, Size(color_morph_width, color_morph_height));
        morphologyEx(src_threshold, src_threshold, MORPH_CLOSE, element);
        src_threshold.copyTo(out);

        vector<vector<Point>> contours;

        findContours(src_threshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

        vector<vector<Point>>::iterator itc = contours.begin();
        while (itc != contours.end()) {
            RotatedRect mr = minAreaRect(Mat(*itc));

            if (!verifySizes(mr))
                itc = contours.erase(itc);
            else {
                ++itc;
                outRects.push_back(mr);
            }          
        }
        return 0;
    }

    bool Tsolpr::PlateLocate::rotation(Mat& in, Mat& out, const Size rect_size, const Point2f center, const double angle)
    {
        Mat in_large;
        in_large.create(int(in.rows * 1.5), int(in.cols * 1.5), in.type());

        float x = in_large.cols / 2 - center.x > 0 ? in_large.cols / 2 - center.x : 0;
        float y = in_large.rows / 2 - center.y > 0 ? in_large.rows / 2 - center.y : 0;

        float width = x + in.cols < in_large.cols ? in.cols : in_large.cols - x;
        float height = y + in.rows < in_large.rows ? in.rows : in_large.rows - y;

        if (width != in.cols || height != in.rows) return false;

        Mat imageRoi = in_large(Rect_<float>(x, y, width, height));
        addWeighted(imageRoi, 0, in, 1, 0, imageRoi);

        Point2f center_diff(in.cols / 2.f, in.rows / 2.f);
        Point2f new_center(in_large.cols / 2.f, in_large.rows / 2.f);

        Mat rot_mat = getRotationMatrix2D(new_center, angle, 1);

        Mat mat_rotated;
        warpAffine(in_large, mat_rotated, rot_mat, Size(in_large.cols, in_large.rows),
            CV_INTER_CUBIC);

        Mat img_crop;
        getRectSubPix(mat_rotated, Size(rect_size.width, rect_size.height),
            new_center, img_crop);

        out = img_crop;

        return true;
    }

    PlateLocate* PlateLocate::instance()
    {
        if (!instance_) {
            instance_ = new PlateLocate;
        }
        return instance_;
    }

    int Tsolpr::PlateLocate::deskew(const Mat& src, const Mat& src_b, std::vector<RotatedRect>& inRects, std::vector<Plate>& outPlates, Color color) {

        Mat mat_debug;
        src.copyTo(mat_debug);

        for (size_t i = 0; i < inRects.size(); i++) {
            RotatedRect InRect_b = inRects[i];

            float r = (float)InRect_b.size.width / (float)InRect_b.size.height;
            float InRect_b_angle = InRect_b.angle;

            Size InRect_b_size = InRect_b.size;
            if (r < 1) {
                InRect_b_angle = 90 + InRect_b_angle;
                swap(InRect_b_size.width, InRect_b_size.height);
            }

            if (InRect_b_angle - 60 < 0 && InRect_b_angle + 60 > 0) {
                Rect_<float> Rect_Revise;
                bool Flag = RectRevise(InRect_b, src, Rect_Revise);
                if (!Flag) continue;

                Mat bound_mat = src(Rect_Revise);
                Mat bound_mat_b = src_b(Rect_Revise);
                Point2f ref_center = InRect_b.center - Rect_Revise.tl();

                Mat deskew_mat;
                if ((InRect_b_angle - 5 < 0 && InRect_b_angle + 5 > 0) || 90.0 == InRect_b_angle ||
                    -90.0 == InRect_b_angle) {
                    deskew_mat = bound_mat;
                }
                else {
                    Mat rotated_mat;
                    Mat rotated_mat_b;

                    if (!rotation(bound_mat, rotated_mat, InRect_b_size, ref_center, InRect_b_angle))
                        continue;

                    if (!rotation(bound_mat_b, rotated_mat_b, InRect_b_size, ref_center, InRect_b_angle))
                        continue;

                    deskew_mat = rotated_mat;
                }

                Mat plate_mat;
                plate_mat.create(HEIGHT, WIDTH, TYPE);

                deleteNotArea(deskew_mat, color);

                if (deskew_mat.cols * 1.0 / deskew_mat.rows > 2.3 && deskew_mat.cols * 1.0 / deskew_mat.rows < 6) {
                    if (deskew_mat.cols >= WIDTH || deskew_mat.rows >= HEIGHT)
                        resize(deskew_mat, plate_mat, plate_mat.size(), 0, 0, INTER_AREA);
                    else
                        resize(deskew_mat, plate_mat, plate_mat.size(), 0, 0, INTER_CUBIC);

                    Plate plate;
                    plate.setPlatePos(InRect_b);
                    plate.setPlateMat(plate_mat);
                    if (color != UNKNOWN) plate.setPlateColor(color);
                    outPlates.push_back(plate);
                }
            }
        }
        return 0;
    }

    int Tsolpr::PlateLocate::ColorLocate(Mat src, std::vector<Plate>& PlateVec, int count)
    {
        vector<RotatedRect> BluePlateRect;
        BluePlateRect.reserve(64);
        vector<RotatedRect> YellowPlateRect;
        YellowPlateRect.reserve(64);

        vector<Plate> plates_blue;
        plates_blue.reserve(64);
        vector<Plate> plates_yellow;
        plates_yellow.reserve(64);
        Mat src_clone = src.clone();

        Mat BluePlate;
        Mat YellowPlate;
#pragma omp parallel sections
        {
#pragma omp section
            {
                ColorSearch(src, BLUE, BluePlate, BluePlateRect);
                deskew(src, BluePlate, BluePlateRect, plates_blue, BLUE);
            }
#pragma omp section
            {
                ColorSearch(src_clone, YELLOW, YellowPlate, YellowPlateRect);
                deskew(src_clone, YellowPlate, YellowPlateRect, plates_yellow, YELLOW);
            }
        }

        PlateVec.insert(PlateVec.end(), plates_blue.begin(), plates_blue.end());
        PlateVec.insert(PlateVec.end(), plates_yellow.begin(), plates_yellow.end());

        return 0;
    }
}



