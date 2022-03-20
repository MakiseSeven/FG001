#pragma once
#include "basic_plate.hpp"

namespace Tsolpr {

    class PlateJudge {
    public:
        static PlateJudge* point();

        void LoadSvmModel(std::string path);

        int plateJudgeUsingNMS(const std::vector<Plate>& inVec, std::vector<Plate>&resultVec, int maxPlates = 5);

        int plateSetScore(Plate& plate);

    private:
        PlateJudge() ;

        static PlateJudge* point_;

        cv::Ptr<ml::SVM> svm_model;

    };
}
