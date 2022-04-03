#include "utils.h"
#include"opencv2/opencv.hpp"
#include <windows.h>
#include <direct.h>
#include"io.h"

namespace Tsolpr {
	std::string utils::get_file_name(const std::string& path)
	{
        if (!path.empty()) {
            size_t last_slash = path.find_last_of('/');
            size_t last_dot = path.find_last_of('.');
            
            if (last_dot < last_slash || last_dot == std::string::npos) {
                return path.substr(last_slash + 1);
            }
            else {
                return path.substr(last_slash + 1, last_dot - last_slash - 1);
            }
        }
        return "";
    }

	std::vector<std::string> utils::get_files(const std::string& path)
	{
        std::vector<std::string> files;
        std::list<std::string> subfolders;
        subfolders.push_back(path);
        while (!subfolders.empty()) {
            std::string current_folder(subfolders.back());

            if (*(current_folder.end() - 1) != '/') {
                current_folder.append("/*");
            }
            else {
                current_folder.append("*");
            }

            subfolders.pop_back();

            struct _finddata_t file_info;
            auto file_handler = _findfirst(current_folder.c_str(), &file_info);

            while (file_handler != -1) {
                if (!strcmp(file_info.name, ".") || !strcmp(file_info.name, "..")){
                    if (_findnext(file_handler, &file_info) != 0) break;
                    continue;
                }

                if (file_info.attrib & _A_SUBDIR) {
                    std::string folder(current_folder);
                    folder.pop_back();
                    folder.append(file_info.name);
                   
                    subfolders.push_back(folder.c_str());

                }
                else {
                    std::string file_path;
                    file_path.assign(current_folder.c_str()).pop_back();
                    file_path.append(file_info.name);

                    files.push_back(file_path);
                }

                if (_findnext(file_handler, &file_info) != 0) break;
            } 
            _findclose(file_handler);
        }
        return files;
	}

	std::string utils::ConvertUtf8ToGBK(const char* strUtf8)
	{
        int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);
        wchar_t* wszGBK = new wchar_t[len + 1];
        memset(wszGBK, 0, len * 2 + 2);
        MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, wszGBK, len);
        len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
        char* GBK = new char[len + 1];
        memset(GBK, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, GBK, len, NULL, NULL);
        std::string strVec(GBK);
        if (wszGBK)
            delete[] wszGBK;
        if (GBK)
            delete[] GBK;
        return strVec;

	}

    std::size_t utils::get_last_slash(const std::string& path)
    {
        size_t last_slash_1 = path.find_last_of("\\");
        size_t last_slash_2 = path.find_last_of("/");
        size_t last_slash;

        if (last_slash_1 != std::string::npos && last_slash_2 != std::string::npos) {
            // C:/path\\to/file.postfix
            last_slash = (std::max)(last_slash_1, last_slash_2);
        }
        else {
            // C:\\path\\to\\file.postfix
            // C:/path/to/file.postfix
            last_slash =
                (last_slash_1 == std::string::npos) ? last_slash_2 : last_slash_1;
        }

        return last_slash;
    }
}