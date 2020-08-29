#include "LangFactory.h"

#include "cplusplus.h"

using namespace kudroma::code_assistant;

std::string LangFactory::cplusplus_{ "cplusplus" };

std::shared_ptr<Lang> kudroma::code_assistant::LangFactory::createLang(const std::string& lang)
{
    if (lang == cplusplus_)
        return std::make_shared<Cplusplus>();
    else
        return nullptr;
}
