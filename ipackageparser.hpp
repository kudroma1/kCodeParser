#pragma once

#include "packagedesc.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include <memory>

namespace kudroma {  namespace code_parser {

    class IPackageParser
    {
    public:
        std::shared_ptr<PackageDesc> tryParse(const std::string& line);
        std::string parseItemName(const std::string& line);

    protected:
        static boost::regex packageRegex_;
    };

}}