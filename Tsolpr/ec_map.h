#pragma once
#include <map>
#include <string>

namespace Tsolpr {

    class ec_map {
    public:
        ec_map() {};

        void load(const std::string& file);

        void add(const std::string& key, const std::string& value);

        std::string get(const std::string& key);

        void clear();

    private:
        std::map<std::string, std::string> data_;
    };

}