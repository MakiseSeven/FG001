#include<python.h>
#include<fstream>
#include "char_classify.h"


namespace Tsolpr {

    CharClassify* CharClassify::instance_ = nullptr;

    CharClassify* CharClassify::instance()
    {
        if (!instance_) {
            instance_ = new CharClassify;
        }
        return instance_;
    }

    std::pair<std::string, std::string> CharClassify::identify(cv::Mat input, bool isChinese, bool isAlphabet)
    {
        cv::Mat feature = AlphaCharFeatures(input, kPredictSize);
        float maxVal = -2;
        auto index = static_cast<int>(classify(feature, maxVal, isChinese, isAlphabet));
        if (index < kCharactersNumber) {
            return std::make_pair(kChars[index], kChars[index]);
        }
        else {
            const char* key = kChars[index];
            std::string s = key;
            std::string province = ec_map_->get(s);
            return std::make_pair(s, province);
        }
    }

    std::pair<std::string, std::string> CharClassify::ClassifyChineseGray(cv::Mat input)
    {
        std::string strImagePath = "temporary_image.jpg";
        imwrite(strImagePath.c_str(), input);
        std::string path = "E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition";
        path = path + "/" + strImagePath;
        Py_SetPythonHome(L"E:/boot_file/anaconda/envs/tensorflow");
        Py_Initialize();
        if (!Py_IsInitialized())
        {
            std::cout << "fail to initial!" << std::endl;
            Py_Finalize();
        }
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('E:/personal_file/test/test2/PythonApplication2/PythonApplication2')");
        PyObject* pModule = NULL;
        PyObject* pFunc = NULL;
        PyObject* pParams = NULL;
        PyObject* pResult = NULL;
        pModule = PyImport_ImportModule("PythonApplication2");
        if (pModule == NULL)
        {
            std::cout << "don't find the python file!" << std::endl;
        }
        pFunc = PyObject_GetAttrString(pModule, "char_reco");
        pParams = Py_BuildValue("(s)", path.c_str());
        pResult = PyEval_CallObject(pFunc, pParams);
        std::ifstream first_file("E:/personal_file/test/test2/PythonApplication2/PythonApplication2/temporary_file.txt");;
        std::string first;
        std::string second;
        getline(first_file, first);
        first_file.close();
        std::ifstream second_file("E:/personal_file/test/test2/PythonApplication2/PythonApplication2/temporary_file2.txt");
        getline(second_file, second);
        second_file.close();
        return std::make_pair(first, second);
    }

    void CharClassify::ClassifyChineseGray(std::vector<Character>& charVec)
    {
        size_t charVecSize = charVec.size();
        if (charVecSize == 0)
            return;

        Mat featureRows;
        for (size_t index = 0; index < charVecSize; index++) {
            Mat charInput = charVec[index].getCharacterMat();
            cv::Mat feature;
            ExtractCharFeature(charInput, feature);
            featureRows.push_back(feature);
        }

        cv::Mat output(charVecSize, kChineseNumber, CV_32FC1);
        annGray_->predict(featureRows,output);

        for (size_t output_index = 0; output_index < charVecSize; output_index++) {
            Character& character = charVec[output_index];
            Mat output_row = output.row(output_index);
            bool isChinese = true;

            float maxVal = -2;
            int result = 0;

            for (int j = 0; j < kChineseNumber; j++) {
                float val = output_row.at<float>(j);
                //std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }

            // no match
            if (-1 == result) {
                result = 0;
                maxVal = 0;
                isChinese = false;
            }

            auto index = result + kCharsTotalNumber - kChineseNumber;
            const char* key = kChars[index];
            std::string s = key;
            std::string province = ec_map_->get(s);

            /*std::cout << "result:" << result << std::endl;
            std::cout << "maxVal:" << maxVal << std::endl;*/

            character.setCharacterScore(maxVal);
            character.setCharacterString(province);
            character.setIsChinese(isChinese);
        }
    }

    CharClassify::CharClassify()
    {
        ann_ = ml::ANN_MLP::load(kDefaultAnnPath);
        annGray_ = ml::ANN_MLP::load(kGrayAnnPath); 

        ec_map_ = std::shared_ptr<ec_map>(new ec_map);
        ec_map_->load(kChineseMappingPath);
    }

    void CharClassify::LoadAnnModel(std::string path)
     {
        if (!ann_->empty()) {
            ann_->clear();
        }
        ann_ = ml::ANN_MLP::load(path);
      }

    void CharClassify::LoadChAnn(std::string path)
    {
        if (annGray_->empty()) {
            annGray_->clear();
        }
        annGray_= ml::ANN_MLP::load(path);
    }

    void CharClassify::LoadChMap(std::string path)
    {
        ec_map_->clear();
        ec_map_->load(path);
    }

	int CharClassify::classify(cv::Mat f, float& maxVal, bool isChinses, bool isAlphabet)
    {
        int result = 0;

        cv::Mat output(1, kCharsTotalNumber, CV_32FC1);
        ann_->predict(f,output);

        maxVal = -2.f;
        if (!isChinses) {
            if (!isAlphabet) {
                result = 0;
                for (int j = 0; j < kCharactersNumber; j++) {
                    float val = output.at<float>(j);
                    // std::cout << "j:" << j << "val:" << val << std::endl;
                    if (val > maxVal) {
                        maxVal = val;
                        result = j;
                    }
                }
            }
            else {
                result = 0;
                // begin with 11th char, which is 'A'
                for (int j = 10; j < kCharactersNumber; j++) {
                    float val = output.at<float>(j);
                    // std::cout << "j:" << j << "val:" << val << std::endl;
                    if (val > maxVal) {
                        maxVal = val;
                        result = j;
                    }
                }
            }
        }
        else {
            result = kCharactersNumber;
            for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
                float val = output.at<float>(j);
                //std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
        //std::cout << "maxVal:" << maxVal << std::endl;
        return result;
    }

}
