#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace kudroma{namespace code_parser{

class ClassDesc;

class IClassParser
{
public:
    virtual std::shared_ptr<ClassDesc> tryParse(const std::string& line) = 0;

protected:
    bool extractProps(const std::string& line, std::unordered_set<std::string>& props) const;
    bool extractFullyQualifiedName(const std::string& line, std::string& fqn) const;

protected:
    static boost::regex classWithPropsRegex_;
    static boost::regex classWithInheritanceWithPropsRegex_;
    static boost::regex classRegex_;
    static boost::regex classWithInheritanceRegex_;
    static boost::regex classNameRegex_;
    static std::unordered_set<std::string> keywords_;
};
}}