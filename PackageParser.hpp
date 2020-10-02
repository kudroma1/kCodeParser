#pragma once

#include <boost/regex.hpp>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace kudroma {  namespace code_assistant {

    class Item;
    class IClassGenerator;
    class PackageParser
    {
    public:
        bool parseProjectTree(const std::string& filename, const fs::path& dir);
        bool buildProjectTree();

    private:
        bool parseLine(const std::string& line);
        std::string parseItemName(const std::string& line);
        std::shared_ptr<Item> createItem(const std::string& line, const std::shared_ptr<Item> parent);
        uint8_t parseHierarchyLevel(const std::string& line);

    private:
        const boost::regex langRegex_{ "\\s*lang [a-zA-Z]*" };
        const boost::regex namespaceRegex_{ "\\s*namespace [a-zA-Z:]*" };
        const boost::regex packageRegex_{ "\\s*pk [a-zA-Z_0-9]*\\s*" };
        const boost::regex classRegex_{ "\\s*cl [a-zA-Z_0-9]*\\s*" };
        const boost::regex classWithInheritanceRegex_{ "(\\s*cl [a-zA-Z_0-9]*\\s*):(\\s*[a-zA-Z_0-9]*\\s*)" };

        uint32_t spaceNum_{ 0 };
        std::shared_ptr<Item> rootItem_{ nullptr };
        std::shared_ptr<Item> parentItem_{ nullptr };
        std::shared_ptr<Item> lastItem_{ nullptr };
        uint8_t curHierarchyLevel_{ 0 };
        std::vector<std::string> namespaces_;

        uint8_t tabSize_{ 4 };

        fs::path dir_;

        std::string lang_;
    };
}}