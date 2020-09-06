#include "cplusplusclassgenerator.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

#include <fstream>

bool kudroma::code_assistant::CplusplusClassGenerator::generateClass(const fs::path& dir, const std::string& classname)
{
    return generateHeader(dir, classname) && generateSource(dir, classname);
}

bool kudroma::code_assistant::CplusplusClassGenerator::generateHeader(const fs::path& dir, const std::string& classname)
{
    std::fstream header;
    auto local = classname;
    boost::to_lower(local);
    const auto headerName = local + ".hpp";
    auto headerPath = dir;
    if (!fs::exists(dir))
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% dir '%2%' doesn't exist") % __FUNCTION__ % dir);
        return false;
    }

    headerPath += headerName;
    if (!fs::exists(headerPath))
    {
        header.open(headerPath, std::ios_base::out);
        if (!header.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% header file '%2%' was not created in '%3%") % __FUNCTION__ % headerName % dir);
            return false;
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% header file '%2%' already exists in '%3%'") % __FUNCTION__ % headerName % dir);

    return true;
}

bool kudroma::code_assistant::CplusplusClassGenerator::generateSource(const fs::path& dir, const std::string& sourcename)
{
    std::fstream source;
    auto local = sourcename;
    boost::to_lower(local);
    const auto sourceName = local + ".cpp";
    auto sourcePath = dir;
    sourcePath += sourceName;
    if (!fs::exists(sourcePath))
    {
        source.open(sourceName, std::ios_base::out);
        if (!source.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% source file '%2%' was not created in '%3%") % __FUNCTION__ % sourceName % dir);
            return false;
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% source file '%2%' already exists in '%3%'") % __FUNCTION__ % sourceName % dir);

    return true;
}
