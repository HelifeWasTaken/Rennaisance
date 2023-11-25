#include "renn/Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace renn {

    std::string load_file(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw UtilsException("Failed to open file " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::vector<std::string> split(const std::string& str, const std::string& delim)
    {
        std::vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) {
                pos = str.length();
            }
            std::string token = str.substr(prev, pos - prev);
            if (!token.empty()) {
                tokens.push_back(token);
            }
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    }

    std::string trim(const std::string& str)
    {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    std::string join(const std::vector<std::string>& strings, const std::string& delim)
    {
        std::string result;
        // avoid last delimiter
        for (size_t i = 0; i < strings.size() - 1; ++i) {
            result += strings[i] + delim;
        }
        result += strings[strings.size() - 1];
        return result;
    }

}