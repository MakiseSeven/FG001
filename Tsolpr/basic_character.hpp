#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

namespace Tsolpr {

    class Character {
    public:
        Character()
        {
            c_Mat = Mat();
            c_GrayMat = Mat();
            c_Rect = Rect();
            c_String = "";
            c_Score = 0;
            c_IsChinese = false;
            c_OstuLevel = 125;
            c_Center = Point(0, 0);
            c_Index = 0;
        }

        Character(const Character& other)
        {
            c_Mat = other.c_Mat;
            c_GrayMat = other.c_GrayMat;
            c_Rect = other.c_Rect;
            c_String = other.c_String;
            c_Score = other.c_Score;
            c_IsChinese = other.c_IsChinese;
            c_OstuLevel = other.c_OstuLevel;
            c_Center = other.c_Center;
            c_Index = other.c_Index;
        }

        inline void setCharacterMat(Mat parameter) { c_Mat = parameter; }
        inline Mat getCharacterMat() const { return c_Mat; }

        inline void setCharacterGrayMat(Mat parameter) { c_GrayMat = parameter; }
        inline Mat getCharacterGrayMat() const { return c_GrayMat; }

        inline void setCharacterRect(Rect parameter) { c_Rect = parameter; }
        inline Rect getCharacterRect() const { return c_Rect; }

        inline void setCharacterString(String parameter) { c_String = parameter; }
        inline String getCharacterString() const { return c_String; }

        inline void setCharacterScore(double parameter) { c_Score = parameter; }
        inline double getCharacterScore() const { return c_Score; }

        inline void setIsChinese(bool parameter) { c_IsChinese = parameter; }
        inline bool getIsChinese() const { return c_IsChinese; }

        inline void setOstuLevel(double parameter) { c_OstuLevel = parameter; }
        inline double getOstuLevel() const { return c_OstuLevel; }

        inline void setCenterPoint(Point parameter) { c_Center = parameter; }
        inline Point getCenterPoint() const { return c_Center; }

        inline void setIndex(int parameter) { c_Index = parameter; }
        inline int getIndex() const { return c_Index; }

        inline bool getIsStrong() const { return c_Score >= 0.9; }
        inline bool getIsWeak() const { return c_Score < 0.9 && c_Score >= 0.5; }
        inline bool getIsLittle() const { return c_Score < 0.5; }

        bool operator < (const Character& other) const
        {
            return (c_Score > other.c_Score);
        }

        bool operator < (const Character& other)
        {
            return (c_Score > other.c_Score);
        }

    private:
        Mat c_Mat;

        Mat c_GrayMat;

        Rect c_Rect;

        String c_String;

        double c_Score;

        bool c_IsChinese;

        double c_OstuLevel;

        Point c_Center;

        int c_Index;

    };

} 

