#include "classitem.h"

#include "classgeneratorfactory.h"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

using namespace kudroma::code_assistant;

kudroma::code_assistant::ClassItem::ClassItem(const std::string& name, const std::string& lang, const std::shared_ptr<Item> parent, std::vector<std::string> namespaces) 
    : Item(name, parent), namespaces_(namespaces)
{
    type_ = ItemType::Class;

    classGenerator_ = ClassGeneratorFactory::generateClassGenerator(lang);
    if (!classGenerator_)
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " was not able to generate class for lang='" << lang << "'";
}

bool kudroma::code_assistant::ClassItem::build()
{
    if (classGenerator_)
        return classGenerator_->generate(dir_, name_, namespaces_, IClassGenerator::CLASS);
    else
        return false;
}