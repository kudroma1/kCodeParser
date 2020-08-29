#pragma once

#include <boost/regex.hpp>

#include <string>

namespace kudroma {  namespace code_assistant {

    class Lang;
    class Item;
    class PackageParser
    {
    public:
        // 
        bool parseFromFile(const std::string& filename);

    private:
        bool parseLine(const std::string& line);

    private:
        const boost::regex lang_{ "\s*lang [a-zA-Z]*" };
        const boost::regex package_{ "\s*pk [a-zA-Z]*" };
        const boost::regex class_{ "\s*cl [a-zA-Z]*" };

        uint32_t spaceNum_{ 0 };
        std::shared_ptr<Lang> curLang_{ nullptr };
        std::shared_ptr<Item> curItem_{ nullptr };
    };
}}