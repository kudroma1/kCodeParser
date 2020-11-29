#pragma once

#include <boost/regex.hpp>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace kudroma {  namespace code_parser {

    class Item;
    class IClassGenerator;
    class IClassParser;
    class IPackageParser;
    class ClassDesc;
    class PackageDesc;

    class MainParser
    {
    public:
        bool parseProjectTree(const std::string& filename, const fs::path& dir);
        bool buildProjectTree();

    private:
        void clear();
        bool parseLine(const std::string& line);
        bool parseLang(const std::string& line);
        uint8_t parseHierarchyLevel(const std::string& line);
        bool handleClass(uint8_t level, const std::shared_ptr<ClassDesc> desc);
        bool handlePackage(uint8_t level, const std::shared_ptr<PackageDesc> desc);
        bool handleHierarchyLevel(uint8_t level);

    private:
        const boost::regex langRegex_{ "\\s*lang [a-zA-Z]*" };

        uint32_t spaceNum_{ 0 };
        std::shared_ptr<Item> root_{ nullptr };
        std::shared_ptr<Item> parentItem_{ nullptr };
        std::shared_ptr<Item> lastItem_{ nullptr };
        uint8_t curHierarchyLevel_{ 0 };
        std::vector<std::string> namespaces_;
        uint8_t tabSize_{ 4 };
        fs::path dir_;
        std::string lang_;
        std::shared_ptr<IClassParser> classParser_{ nullptr };
        std::shared_ptr<IPackageParser> packageParser_{ nullptr };
        std::shared_ptr<IClassGenerator> classGenerator_{ nullptr };
    };
}}