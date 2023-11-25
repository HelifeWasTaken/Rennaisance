#pragma once

/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Utils.h
License: GNU v3.0
*/

#include <string>
#include <vector>
#include <stdexcept>

namespace renn {

    std::string load_file(const std::string& path);
    std::vector<std::string> split(const std::string& str, const std::string& delim);
    std::string trim(const std::string& str);
    std::string join(const std::vector<std::string>& strings, const std::string& delim);

    class UtilsException : public std::runtime_error {
    public:
        UtilsException(const std::string& message) : std::runtime_error(message) {}
    };

}