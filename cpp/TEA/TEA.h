#pragma once

#include <stdint.h>
#include <string>
#include <string.h>

class TEA {
public:
    static std::string encrypt(const std::string& in, const std::string& key);
    static std::string decrypt(const std::string& in, const std::string& key);
};