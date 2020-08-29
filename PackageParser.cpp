#include "PackageParser.hpp"

#include "classitem.h"
#include "lang.h"
#include "LangFactory.h"
#include "packageitem.h"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include <fstream>

using namespace kudroma::code_assistant;

bool PackageParser::parseFromFile(const std::string& filename)
{
    QDir curDir;

    std::ifstream file;
    file.open(filename);
    if (file.is_open())
    {
        std::string line;
        uint32_t lineNum{ 1 };
        spaceNum_ = 0;
        curLang_ = nullptr;
        curItem_ = nullptr;

        // check lang
        std::getline(file, line);
        if (boost::regex_match(line, lang_))
        {
            std::vector<std::string> parts;
            boost::split(parts, line, boost::is_any_of(" "));
            curLang_ = LangFactory::createLang(parts.back());
            if (!curLang_)
            {
                BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("Line%1% in \"%3%\": Lang \"%2%\" is not known") % lineNum % parts.back() % filename);
                return false;
            }
            else
                BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Lang : %1%") % parts.back());
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("Line%1% in \"%2%\": \"%1%\" doesn't contain lang") % line % filename);
            return false;
        }

        // check content
        while (!file.eof())
        {
            lineNum++;
            std::getline(file, line);
            parseLine(line);
        }
    }
    else
    {
        file.close();
        BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("File \"%1%\" was not opened") % filename);
        return false;
    }

    file.close();
    return true;
}

bool kudroma::code_assistant::PackageParser::parseLine(const std::string& line)
{
    // Handle top-level package
    if (!curItem_ && boost::regex_match(line, package_))
    {
        BOOST_LOG_TRIVIAL(debug) << "First item is package";
        return true;
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" doesn't contain package") % line);
        return false;
    }
}