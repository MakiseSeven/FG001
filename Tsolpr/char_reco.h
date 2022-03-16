#pragma once
#include"sundry_function.h"
#include"char_segmentation.h"
namespace Tsolpr {

    class CharsReco {
    public:
        CharsReco();

        ~CharsReco();

        int charsRecognise(Plate& plate, std::string& plateLicense);

        inline std::string getPlateColor(cv::Mat input) const {
            std::string color = "δ֪";
            Color result = getPlateType(input, true);
            if (BLUE == result) color = "����";
            if (YELLOW == result) color = "����";
            if (WHITE == result) color = "����";
            return color;
        }

        inline std::string getPlateColor(Color in) const {
            std::string color = "δ֪";
            if (BLUE == in) color = "����";
            if (YELLOW == in) color = "����";
            if (WHITE == in) color = "����";
            return color;
        }

    private:

       static CharsSegment* charSegment;
    };

}

