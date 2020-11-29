#include "classutilityfactory.hpp"

#include "cplusplusclassgenerator.hpp"
#include "cplusplusclassparser.hpp"
#include "ipackageparser.hpp"
#include "iclassparser.h"

#include <boost/algorithm/string.hpp>

using namespace kudroma::code_parser;

std::set<std::string> ClassUtilityFactory::cplusplusPattern_ = { "cplusplus", "c++" };

std::shared_ptr<IClassGenerator> kudroma::code_parser::ClassUtilityFactory::generateClassGenerator(const std::string& lang)
{
    auto local = lang;
    boost::to_lower(local);
    if(cplusplusPattern_.find(local) != cplusplusPattern_.cend())
        return std::make_shared<CplusplusClassGenerator>();
    else
        return nullptr;
}

std::shared_ptr<IClassParser> kudroma::code_parser::ClassUtilityFactory::generateClassParser(const std::string& lang)
{
    auto local = lang;
    boost::to_lower(local);
    if (cplusplusPattern_.find(local) != cplusplusPattern_.cend())
        return std::make_shared<CplusplusClassParser>();
    else
        return nullptr;
}

std::shared_ptr<IPackageParser> kudroma::code_parser::ClassUtilityFactory::generatePackageParser(const std::string& lang)
{
    auto local = lang;
    boost::to_lower(local);
    if (cplusplusPattern_.find(local) != cplusplusPattern_.cend())
        return std::make_shared<IPackageParser>();
    else
        return nullptr;
}
