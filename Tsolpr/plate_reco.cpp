#include"plate_reco.h"
#include"char_classify.h"
#include"plate_judge.h"
namespace Tsolpr {
    int PlateRecognize::plateRecognize(const Mat& src, std::vector<Plate>& plateVec, int count)
    {
        float scale = 1.0f;
        Mat resize_img = Resize(src, scale);
        std::vector<Plate> plate;
        int result = plateDetect(resize_img, plate, 0, false, count);
        if (result == 0) {
            size_t num = plate.size();
            for (size_t j = 0; j < num; j++) {
                Plate& item = plate.at(j);
                Mat plateMat = item.getPlateMat();
   
                item.setPlateScale(scale);
                RotatedRect rect = item.getPlatePos();
                item.setPlatePos(rectResize(rect, 1.f / scale));

                Color color = item.getPlateColor();
                if (color == UNKNOWN) {
                    color = getPlateType(plateMat, true);
                    item.setPlateColor(color);
                }
                std::string plateColor = getPlateColor(color);

                std::string plateIdentify ;
                int resultCR = charsRecognise(item, plateIdentify);
                if (resultCR == 0) {
                    std::string license = plateColor + ":" + plateIdentify;
                    item.setPlateStr(license);
                    plateVec.push_back(item);
                }
                else {
                    std::string license = plateColor;
                    item.setPlateStr(license);
                    plateVec.push_back(item);
                }
            }
        }
        return result;
    }
    void PlateRecognize::LoadSVM(std::string path)
    {
        PlateJudge::point()->LoadSvmModel(path);
    }

    void PlateRecognize::LoadANN(std::string path)
	{
        CharClassify::instance()->LoadAnnModel(path);
       
	}

    void PlateRecognize::LoadChAnn(std::string path)
    {
        CharClassify::instance()->LoadChAnn(path);
    }

    void PlateRecognize::LoadChMap(std::string path)
    {
        CharClassify::instance()->LoadChMap(path);
    }

}