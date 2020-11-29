#pragma once

#include <string>

namespace kudroma {  namespace code_parser {

    class PackageDesc
    {
    public:
        PackageDesc(const std::string& nameVal) : name(nameVal) {}
        std::string name;
    };

 }}