#include "plate_judge.h"
#include"sundry_function.h"
#include"FeatureCreate.h"
namespace Tsolpr {

    PlateJudge* PlateJudge::point_ = nullptr;

    PlateJudge::PlateJudge() {
        svm_model = ml::SVM::load("E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/svm_hist.xml");
    }

    PlateJudge* Tsolpr::PlateJudge::point()
    {
        if (!point_) {
            point_ = new PlateJudge;
        }
        return point_;

    }

    void Tsolpr::PlateJudge::LoadSvmModel(std::string path) {
        if (path != std::string(kDefaultSvmPath)) {
            if (!svm_model->empty()) svm_model->clear();
            svm_model = ml::SVM::load(path);
        }
    }

    int Tsolpr::PlateJudge::plateJudgeUsingNMS(const std::vector<Plate>& inVec, std::vector<Plate>& resultVec, int maxPlates)
    {
        std::vector<Plate> plateVec;
        int num = inVec.size();
        bool useCascadeJudge = true;

        for (int j = 0; j < num; j++) {
            Plate plate = inVec[j];
            Mat inMat = plate.getPlateMat();
            int result = plateSetScore(plate);
            if (0 == result) {
                plateVec.push_back(plate);
            }
        }

        std::vector<Plate> reDupPlateVec;
        double overlap = 0.5;
        NMS(plateVec, reDupPlateVec, overlap);
        std::sort(reDupPlateVec.begin(), reDupPlateVec.end());
        std::vector<Plate>::iterator it = reDupPlateVec.begin();
        int count = 0;
        for (; it != reDupPlateVec.end(); ++it) {
            resultVec.push_back(*it);
            count++;
            if (count >= maxPlates)
                break;
        }
        return 0;
    }

    int PlateJudge::plateSetScore(Plate& plate)
    {
        Mat features;
        FeatureCreate(plate.getPlateMat(), features);
        float score = svm_model->predict(features, noArray(), cv::ml::StatModel::Flags::RAW_OUTPUT);
        plate.setPlateScore(score);
        if (score < 0.5) return 0;
        else return -1;
    }
}
