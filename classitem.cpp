#include "classitem.h"

#include "ClassDesc.h"
#include "classutilityfactory.hpp"
#include "iclassgenerator.hpp"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

using namespace kudroma::code_parser;

kudroma::code_parser::ClassItem::ClassItem(const std::shared_ptr<ClassDesc> desc, const std::shared_ptr<Item> parent) 
    : Item(parent)
{
    type_ = ItemType::Class;
    if(!desc)
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " ClassDesc is nullptr";
    else
    {
        desc_ = desc;
        name_ = desc->name;
        classGenerator_ = ClassUtilityFactory::generateClassGenerator(desc->lang);
        if (!classGenerator_)
            BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " was not able to generate class for lang='" << desc->lang << "'";
    }
}

bool kudroma::code_parser::ClassItem::build()
{
    if (classGenerator_)
        return classGenerator_->generate(dir_, desc_, IClassGenerator::CLASS);
    else
        return false;
}