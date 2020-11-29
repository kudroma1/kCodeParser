#include "iclassparser.h"

using namespace kudroma::code_parser;

boost::regex IClassParser::classWithPropsRegex_ = boost::regex("\\s*(cl|dcl) [a-zA-Z_0-9]+(::[a-zA-Z_0-9]*)*\\s[a-zA-Z_0-9&=\\s]*");
boost::regex IClassParser::classWithInheritanceWithPropsRegex_ = boost::regex("\\s*(cl|dcl) [a-zA-Z_0-9]+(::[a-zA-Z_0-9]+)*\\s*:\\s*[a-zA-Z_=&0-9\\s]+");
boost::regex IClassParser::classRegex_ = boost::regex("\\s*(cl|dcl) [a-zA-Z_0-9]+(::[a-zA-Z_0-9]+)*");
boost::regex IClassParser::classWithInheritanceRegex_ = boost::regex("\\s*(cl|dcl) [a-zA-Z_0-9]+(::[a-zA-Z_0-9]*)*\\s*:\\s*[a-zA-Z_0-9]+");
boost::regex IClassParser::classNameRegex_ = boost::regex("[a-zA-Z_0-9]+(::[a-zA-Z_0-9]*)*");
std::unordered_set<std::string> IClassParser::keywords_ = {
    "cl", 
    "dcl", 
    "pk", 
    "pimpl",
    "d",
    "vd",
    "c",
    "&",
    "&&",
    "=",
    "=&&"
};

bool kudroma::code_parser::IClassParser::extractProps(const std::string& line, std::unordered_set<std::string>& props) const
{
    if (boost::regex_search(line, classWithInheritanceWithPropsRegex_))
    {
        boost::split(props, boost::regex_replace(line, classWithInheritanceWithPropsRegex_, ""), boost::is_any_of(" "));
        return true;
    }
    else if (boost::regex_search(line, classWithPropsRegex_))
    {
        boost::split(props, boost::regex_replace(line, classRegex_, ""), boost::is_any_of(" "));
        return true;
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " line" << line << " is not class ";
        return false;
    }
}

bool kudroma::code_parser::IClassParser::extractFullyQualifiedName(const std::string& line, std::string& fqn) const
{
    std::string result;
    std::string::const_iterator start, end;
    start = line.begin();
    end = line.end();
    static boost::match_results < std::string::const_iterator > what;
    static boost::match_flag_type flags = boost::match_default;
    while (boost::regex_search(start, end, what, classNameRegex_, flags))
    {
        result = what[0];
        if (!result.empty() && keywords_.find(result) == keywords_.cend())
        {
            fqn = result;
            return true;
        }
        start = what[0].second;
    }

    BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " line" << line << " is not class ";
    return false;
}