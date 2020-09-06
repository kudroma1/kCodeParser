#include "iclassgenerator.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

#include <fstream>

using namespace kudroma::code_assistant;

bool IClassGenerator::generate(const fs::path& dir, std::string& name, const std::uint_fast8_t& properties)
{
    if (properties & CLASS)
        return generateClass(dir, name);
    if (properties & HEADER && properties & SOURCE)
        return generateSource(dir, name) && generateHeader(dir, name);
    if(properties & HEADER)
        return generateHeader(dir, name);
    if (properties & SOURCE)
        return generateSource(dir, name);
    if (properties & EMPTY_FILE)
        return generateEmptyFile(dir, name);

    // generates nothing
    return false;
}

bool kudroma::code_assistant::IClassGenerator::generateEmptyFile(const fs::path& dir, const std::string& filename)
{
    std::fstream file;
    auto temp = filename;
    boost::to_lower(temp);
    auto sourcePath = dir;
    if (!fs::exists(sourcePath))
    {
        file.open(temp, std::ios_base::out);
        if (!file.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% empty file '%2%' was not created in '%3%") % __FUNCTION__ % temp % dir);
            return false;
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% empty file '%2%' already exists in '%3%'") % __FUNCTION__ % temp % dir);

    return true;
}
