#include "ec_map.h"
#include"utils.h"
#include <fstream>
#include <iostream>
void Tsolpr::ec_map::load(const std::string& file)
{
    this->clear();
    std::ifstream reader(file);

    if (reader.is_open()) {
        while (!reader.eof()) {
            std::string line;
            std::getline(reader, line);
            if (line.empty()) continue;

            const auto parse = [](const std::string& str) {
                std::string tmp, key, value;
                for (size_t i = 0, len = str.length(); i < len; ++i) {
                    const char ch = str[i];
                    if (ch == ' ') {
                        if (i > 0 && str[i - 1] != ' ' && key.empty()) {
                            key = tmp;
                            tmp.clear();
                        }
                    }
                    else {
                        tmp.push_back(ch);
                    }
                    if (i == len - 1) {
                        value = tmp;
                    }
                }
                return std::make_pair(key, value);
            };

            auto kv = parse(line);
            this->add(kv.first, kv.second);
        }
        reader.close();
    }
}

void Tsolpr::ec_map::add(const std::string& key, const std::string& value)
{
    if (data_.find(key) != data_.end()) {
        fprintf(stderr,
            "[ec_map] find duplicate: %s = %s , ignore\n",
            key.c_str(),
            value.c_str());
    }
    else {
        std::string v(value);
        v = utils::ConvertUtf8ToGBK(value.c_str());
        data_[key] = v;
    }
}

std::string Tsolpr::ec_map::get(const std::string& key)
{
    if (data_.find(key) == data_.end()) {
        std::cerr << "[ec_map] cannot find " << key << std::endl;
        return "";
    }
    return data_.at(key);
}

void Tsolpr::ec_map::clear()
{
    data_.clear();
}
