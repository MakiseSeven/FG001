#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include"opencv2/opencv.hpp"

namespace Tsolpr {
	class utils {
	 public:
		static  std::string get_file_name(const std::string& path);
	
		static std::vector<std::string> get_files(const std::string& path);

		static std::string ConvertUtf8ToGBK(const char* strUtf8);

		template<typename T>
		 T min(const T& v1, const T& v2) {
			return (v1 < v2) ? v1 : v2;
		}
	private:
		static std::size_t get_last_slash(const std::string& path);

	};

}