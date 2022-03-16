#pragma once
namespace Tsolpr {

    enum Color { BLUE, YELLOW, WHITE, UNKNOWN };

    enum LocateType { SOBEL, COLOR, CMSER, OTHER };

    enum CharSearchDirection { LEFT, RIGHT };

    enum
    {
        PR_MODE_UNCONSTRAINED,
        PR_MODE_CAMERPOHNE,
        PR_MODE_PARKING,
        PR_MODE_HIGHWAY
    };

    enum
    {
        PR_DETECT_SOBEL = 0x01, 
        PR_DETECT_COLOR = 0x02, 
        PR_DETECT_CMSER = 0x04, 
    };

    static const char* kDefaultSvmPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/svm_hist.xml";
    static const char* kLBPSvmPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/svm_lbp.xml";
    static const char* kHistSvmPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/svm_hist.xml";

    static const char* kDefaultAnnPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/ann.xml";
    static const char* kChineseAnnPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/ann_chinese.xml";
    static const char* kGrayAnnPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/annCh.xml";

    //This is important to for key transform to chinese
    static const char* kChineseMappingPath = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/province_mapping";

    typedef enum {
        kForward = 1, // correspond to "has plate"
        kInverse = 0  // correspond to "no plate"
    } SvmLabel;

    static const int   kPlateResizeWidth = 136;
    static const int   kPlateResizeHeight = 36;

    static const int   kShowWindowWidth = 1000;
    static const int   kShowWindowHeight = 800;

    static const float kSvmPercentage = 0.7f;

    static const int   kCharacterInput = 120;
    static const int   kChineseInput = 440;
    static const int   kAnnInput = kCharacterInput;

    static const int   kCharacterSize = 10;
    static const int   kChineseSize = 20;
    static const int   kPredictSize = kCharacterSize;

    static const int   kNeurons = 40;

    static const char* kChars[] = {
      "0", "1", "2",
      "3", "4", "5",
      "6", "7", "8",
      "9",
      /*  10  */
      "A", "B", "C",
      "D", "E", "F",
      "G", "H", /* {"I", "I"} */
      "J", "K", "L",
      "M", "N", /* {"O", "O"} */
      "P", "Q", "R",
      "S", "T", "U",
      "V", "W", "X",
      "Y", "Z",
      /*  24  */
      "zh_chuan" , "zh_e"    , "zh_gan"  ,
      "zh_gan1" , "zh_gui"  , "zh_gui1" ,
      "zh_hei"  , "zh_hu"   , "zh_ji"   ,
      "zh_jin"  , "zh_jing" , "zh_jl"   ,
      "zh_liao" , "zh_lu"   , "zh_meng" ,
      "zh_min"  , "zh_ning" , "zh_qing" ,
      "zh_qiong", "zh_shan" , "zh_su"   ,
      "zh_sx"   , "zh_wan"  , "zh_xiang",
      "zh_xin"  , "zh_yu"   , "zh_yu1"  ,
      "zh_yue"  , "zh_yun"  , "zh_zang" ,
      "zh_zhe"
      /*  31  */
    };

    static const int kCharactersNumber = 34;
    static const int kChineseNumber = 31;
    static const int kCharsTotalNumber = 65;

    static bool kDebug = false;

    static const int kGrayCharWidth = 20;
    static const int kGrayCharHeight = 32;
    static const int kCharLBPGridX = 4;
    static const int kCharLBPGridY = 4;
    static const int kCharLBPPatterns = 16;

    static const int kCharHiddenNeurans = 64;

    static const int kCharsCountInOnePlate = 7;
    static const int kSymbolsCountInChinesePlate = 6;

    static const float kPlateMaxSymbolCount = 7.5f;
    static const int kSymbolIndex = 2;
#define SHOW_IMAGE(imgName, debug) \
  if (debug) { \
    namedWindow("imgName", WINDOW_AUTOSIZE); \
    moveWindow("imgName", 500, 500); \
    imshow("imgName", imgName); \
    waitKey(0); \
    destroyWindow("imgName"); \
  }

}