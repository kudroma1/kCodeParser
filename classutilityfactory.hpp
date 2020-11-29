#pragma once

#include <memory>
#include <set>
#include <string>

namespace kudroma { namespace code_parser{

    class IClassParser;
    class IPackageParser;
    class IClassGenerator;

    class ClassUtilityFactory
    {
    public:
        static std::shared_ptr<IClassGenerator> generateClassGenerator(const std::string& lang);
        static std::shared_ptr<IClassParser> generateClassParser(const std::string& lang);
        static std::shared_ptr<IPackageParser> generatePackageParser(const std::string& lang);

    private:
        static std::set<std::string> cplusplusPattern_;
};

}}