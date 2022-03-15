#pragma once
#include <memory>
#include "opencv2/opencv.hpp"
#include "ec_map.h"
#include "basic_character.hpp"
#include "FeatureCreate.h"

namespace Tsolpr {

    class CharClassify {
    public:
        static CharClassify* instance();

        int classify(cv::Mat f, float& maxVal, bool isChinses = false, bool isAlphabet = false);

        std::pair<std::string, std::string> identify(cv::Mat input, bool isChinese = false, bool isAlphabet = false);

        std::pair<std::string, std::string> ClassifyChineseGray(cv::Mat input);
        void ClassifyChineseGray(std::vector<Character>& charVec);

        void LoadAnnModel(std::string path);

        void LoadChAnn(std::string path);

        void LoadChMap(std::string path);

    private:
        CharClassify();

        static CharClassify* instance_;

        cv::Ptr<cv::ml::ANN_MLP> ann_;

        cv::Ptr<cv::ml::ANN_MLP> annGray_;

        std::shared_ptr<ec_map> ec_map_;
    };
}