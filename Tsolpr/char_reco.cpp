#include "char_reco.h"
#include"char_classify.h"

namespace Tsolpr{
    CharsSegment* CharsReco::charSegment = nullptr;

    CharsReco::CharsReco()
    {
        charSegment = new CharsSegment();
    }

    CharsReco::~CharsReco()
    {
        if (charSegment) {
            delete(charSegment);
            charSegment = nullptr;
        }
    }

    int Tsolpr::CharsReco::charsRecognise(Plate& plate, std::string& plateLicense)
{
    std::vector<Mat> matChars;
    std::vector<Mat> grayChars;
    Mat plateMat = plate.getPlateMat();
    Color color;
    int w = plateMat.cols;
    int h = plateMat.rows;
    Mat tmpMat = plateMat(Rect_<double>(w * 0.1, h * 0.1, w * 0.8, h * 0.8));
    color = getPlateType(tmpMat, true);

    int result = charSegment->charsSegmentUsingOSTU(plateMat, matChars, grayChars, color);

    if (result == 0) {
        int num = matChars.size();
        for (int j = 0; j < num; j++)
        {
            Mat charMat = matChars.at(j);
            Mat grayChar = grayChars.at(j);
            if (color != Color::BLUE)
                grayChar = 255 - grayChar;

            bool isChinses = false;
            std::pair<std::string, std::string> character;
            if (0 == j) {
                isChinses = true;                
                character = CharClassify::instance()->ClassifyChineseGray(grayChar);
                plateLicense.append(character.second);

                // set plate chinese mat and str
                plate.setChineseMat(grayChar);
                plate.setChineseKey(character.first);
            }
            else if (1 == j) {
                isChinses = false;
                bool isAbc = true;
                character = CharClassify::instance()->identify(charMat, isChinses, isAbc);
                plateLicense.append(character.second);
            }
            else {
                isChinses = false;
                character = CharClassify::instance()->identify(charMat, isChinses);
                plateLicense.append(character.second);
            }

            Character charResult;
            charResult.setCharacterMat(charMat);
            charResult.setCharacterGrayMat(grayChar);
            if (isChinses)
                charResult.setCharacterString(character.first);
            else
                charResult.setCharacterString(character.second);

            plate.addReutCharacter(charResult);
        }
        if (plateLicense.size() < 7) {
            return -1;
        }
    }

    return result;
}
}
