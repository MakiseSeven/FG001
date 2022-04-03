#pragma once
#include<iostream>
#include<string>
#include<vector>
#include "char_reco.h"
#include"config.h"

namespace Tsolpr {

    class PlateRecognize : public CharsReco {
    public:
        PlateRecognize() {};

        int plateRecognize(const Mat& src, std::vector<Plate>& plateVec, int count = 0);

        inline void setResultShow(bool parameter) { m_showResult = parameter; }
        inline bool getResultShow() const { return m_showResult; }
        inline void setDebug(bool parameter) { setResultShow(parameter); }

        void LoadSVM(std::string path);
        void LoadANN(std::string path);

        //v1.6 added
        void LoadChAnn(std::string path);
        void LoadChMap(std::string path);

    private:
        bool m_showResult;
    };

} 