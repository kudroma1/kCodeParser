#pragma once

#include <boost/regex.hpp>

#include <string>

namespace kudroma {  namespace code_assistant {

    class Item;
    class IClassGenerator;
    class PackageParser
    {
    public:
        // 
        bool parseFromFile(const std::string& filename);

    private:
        bool parseLine(const std::string& line);
        std::string parseItemName(const std::string& line);
        bool createPackageItem(const std::string& name);
        uint8_t parseHierarchyLevel(const std::string& line);

    private:
        const boost::regex lang_{ "\\s*lang [a-zA-Z]*" };
        const boost::regex package_{ "\\s*pk [a-zA-Z]*" };
        const boost::regex class_{ "\\s*cl [a-zA-Z]*" };

        uint32_t spaceNum_{ 0 };
        std::shared_ptr<Item> curItem_{ nullptr };
        std::shared_ptr<IClassGenerator> classGenerator_{ nullptr };
        uint8_t curHierarchyLevel_{ 0 };
    };
}}