#include "classgeneratorfactory.h"

#include "cplusplusclassgenerator.hpp"

#include <boost/algorithm/string.hpp>

using namespace kudroma::code_assistant;

std::set<std::string> ClassGeneratorFactory::cplusplusPattern_ = { "cplusplus", "c++" };

std::shared_ptr<IClassGenerator> kudroma::code_assistant::ClassGeneratorFactory::generateClassGenerator(const std::string& lang)
{
    auto local = lang;
    boost::to_lower(local);
    if(cplusplusPattern_.find(local) != cplusplusPattern_.cend())
        return std::make_shared<CplusplusClassGenerator>();
    else
        return nullptr;
}
