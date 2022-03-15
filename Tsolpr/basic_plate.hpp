#pragma once
#include"basic_character.hpp"
#include "opencv2/opencv.hpp"
#include"config.h"
namespace Tsolpr {

    class Plate {
    public:
        Plate() {
            Score = -1;
            PlateString = "";
            PlateColor = UNKNOWN;
        }

        Plate(const Plate& other) {
            PlateMat = other.PlateMat;
            ChineseMat = other.ChineseMat;
            ChineseKey = other.ChineseKey;
            Score = other.Score;
            PlateRect = other.PlateRect;
            PlateString = other.PlateString;
            LocateType = other.LocateType;
            PlateColor = other.PlateColor;
            Line = other.Line;
            LeftPoint = other.LeftPoint;
            RightPoint = other.RightPoint;
            MergeCharRect = other.MergeCharRect;
            MaxCharRect = other.MaxCharRect;
            Scale = other.Scale;
            DistVec = other.DistVec;

            MserCharVec = other.MserCharVec;
            ReutCharVec = other.ReutCharVec;
            OstuLevel = other.OstuLevel;
        }

        Plate& operator=(const Plate& other) {
            if (this != &other) {
                PlateMat = other.PlateMat;
                ChineseMat = other.ChineseMat;
                ChineseKey = other.ChineseKey;
                Score = other.Score;
                PlateRect = other.PlateRect;
                PlateString = other.PlateString;
                LocateType = other.LocateType;
                PlateColor = other.PlateColor;
                Line = other.Line;
                LeftPoint = other.LeftPoint;
                RightPoint = other.RightPoint;
                MergeCharRect = other.MergeCharRect;
                MaxCharRect = other.MaxCharRect;

                DistVec = other.DistVec;

                MserCharVec = other.MserCharVec;
                ReutCharVec = other.ReutCharVec;
                OstuLevel = other.OstuLevel;
            }
            return *this;
        }

        inline void setPlateMat(Mat parameter) { PlateMat = parameter; }
        inline Mat getPlateMat() const { return PlateMat; }

        inline void setChineseMat(Mat parameter) { ChineseMat = parameter; }
        inline Mat getChineseMat() const { return ChineseMat; }

        inline void setChineseKey(String parameter) { ChineseKey = parameter; }
        inline String getChineseKey() const { return ChineseKey; }

        inline void setPlatePos(RotatedRect parameter) { PlateRect = parameter; }
        inline RotatedRect getPlatePos() const { return PlateRect; }

        inline void setPlateStr(String parameter) { PlateString = parameter; }
        inline String getPlateStr() const { return PlateString; }

        inline void setPlateLocateType(LocateType parameter) { LocateType = parameter; }
        inline LocateType getPlateLocateType() const { return LocateType; }

        inline void setPlateColor(Color parameter) { PlateColor = parameter; }
        inline Color getPlateColor() const { return PlateColor; }

        inline void setPlateScale(float parameter) { Scale = parameter; }
        inline float getPlateScale() const { return Scale; }

        inline void setPlateScore(double parameter) { Score = parameter; }
        inline double getPlateScore() const { return Score; }

        inline void setPlateLine(Vec4f parameter) { Line = parameter; }
        inline Vec4f getPlateLine() const { return Line; }

        inline void setPlateLeftPoint(Point parameter) { LeftPoint = parameter; }
        inline Point getPlateLeftPoint() const { return LeftPoint; }

        inline void setPlateRightPoint(Point parameter) { RightPoint = parameter; }
        inline Point getPlateRightPoint() const { return RightPoint; }

        inline void setPlateMergeCharRect(Rect parameter) { MergeCharRect = parameter; }
        inline Rect getPlateMergeCharRect() const { return MergeCharRect; }

        inline void setPlateMaxCharRect(Rect parameter) { MaxCharRect = parameter; }
        inline Rect getPlateMaxCharRect() const { return MaxCharRect; }

        inline void setPlatDistVec(Vec2i parameter) { DistVec = parameter; }
        inline Vec2i getPlateDistVec() const { return DistVec; }

        inline void setOstuLevel(double parameter) { OstuLevel = parameter; }
        inline double getOstuLevel() const { return OstuLevel; }

        inline void setMserCharacter(const std::vector<Character>& parameter) { MserCharVec = parameter; }
        inline void addMserCharacter(Character parameter) { MserCharVec.push_back(parameter); }
        inline std::vector<Character> getCopyOfMserCharacters() { return MserCharVec; }

        inline void setReutCharacter(const std::vector<Character>& parameter) { ReutCharVec = parameter; }
        inline void addReutCharacter(Character parameter) { ReutCharVec.push_back(parameter); }
        inline std::vector<Character> getCopyOfReutCharacters() { return ReutCharVec; }

        bool operator < (const Plate& plate) const { return (Score < plate.Score); }
        bool operator < (const Plate& plate) { return (Score < plate.Score); }

    private:
        Mat PlateMat;

        RotatedRect PlateRect;

        String PlateString;

        LocateType LocateType;

        Color PlateColor;

        float Scale;

        double Score;

        double OstuLevel;

        Vec4f Line;

        Point LeftPoint;
        Point RightPoint;

        Rect MergeCharRect;
        Rect MaxCharRect;

        std::vector<Character> MserCharVec;
        std::vector<Character> SlwdCharVec;

        std::vector<Character> OstuCharVec;
        std::vector<Character> ReutCharVec;

        int CharCount;

        Mat ChineseMat;

        String ChineseKey;

        Vec2i DistVec;
    };

}