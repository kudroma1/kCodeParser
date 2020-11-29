#include "cplusplusclassparser.hpp"
#include "ClassDesc.h"

using namespace kudroma::code_parser;

std::shared_ptr<ClassDesc> kudroma::code_parser::CplusplusClassParser::tryParse(const std::string& line)
{
    if (boost::regex_match(line, classWithPropsRegex_)
    || boost::regex_match(line, classWithInheritanceWithPropsRegex_)
    || boost::regex_match(line, classRegex_)
    || boost::regex_match(line, classWithInheritanceRegex_))
    {
        std::unordered_set<std::string> props;
        std::string fullyQualifiedName;
        if (!extractProps(line, props))
        {
            BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Props were not extracted in line " << line;
            return nullptr;
        }
        else if (!extractFullyQualifiedName(line, fullyQualifiedName))
        {
            BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Fully qualified name was not extracted in line " << line;
            return nullptr;
        }
        else 
        {
            auto classDesc = std::make_shared<CplusplusClassDesc>();
            classDesc->props = props;

            std::vector<std::string> namespaces;
            boost::split(namespaces, fullyQualifiedName, boost::is_any_of("::"));
            classDesc->name = namespaces.back();
            namespaces.pop_back();
            for (const auto& ns : namespaces)
            {
                if (!ns.empty())
                    classDesc->namespaces.push_back(ns);
            }
            return classDesc;
        }
    }
    else
        return nullptr;
}