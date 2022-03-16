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
            std::string color = "Î´Öª";
            Color result = getPlateType(input, true);
            if (BLUE == result) color = "À¶ÅÆ";
            if (YELLOW == result) color = "»ÆÅÆ";
            if (WHITE == result) color = "°×ÅÆ";
            return color;
        }

        inline std::string getPlateColor(Color in) const {
            std::string color = "Î´Öª";
            if (BLUE == in) color = "À¶ÅÆ";
            if (YELLOW == in) color = "»ÆÅÆ";
            if (WHITE == in) color = "°×ÅÆ";
            return color;
        }

    private:

       static CharsSegment* charSegment;
    };

}

