#include "PackageParser.hpp"

#include "classitem.h"
#include "classgeneratorfactory.h"
#include "packageitem.h"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include <fstream>

using namespace kudroma::code_assistant;

bool PackageParser::parseFromFile(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);
    if (file.is_open())
    {
        std::string line;
        uint32_t lineNum{ 1 };
        spaceNum_ = 0;
        curItem_ = nullptr;
        classGenerator_ = nullptr;
        curHierarchyLevel_ = 0;

        // check lang
        std::getline(file, line);
        if (boost::regex_match(line, lang_))
        {
            std::vector<std::string> parts;
            boost::split(parts, line, boost::is_any_of(" "));
            classGenerator_ = ClassGeneratorFactory::generateClassGenerator(parts.back());
            if (!classGenerator_)
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
        curHierarchyLevel_ = parseHierarchyLevel(line);
        const auto name = parseItemName(line);
        if (name.size())
            return createPackageItem(name);
        else
        {
            BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" doesn't contain package name") % line);
            return false;
        }
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" doesn't contain package") % line);
        return false;
    }

    const auto level = parseHierarchyLevel(line);
    if (level < curHierarchyLevel_)
    {
        if (!curItem_->parent().expired())
            curItem_ = curItem_->parent().lock();
        else
        {
            curItem_ = nullptr;
            return true;
        }
    }

    if (boost::regex_match(line, package_))
    {
        std::vector<std::string> parts;
        boost::split(parts, line, boost::is_any_of(" "));
        if (parts.size() >= 2)
            return createPackageItem(parts[1]);
        else
            BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" invalid package line '%2%'") % __FUNCTION__ % line);
    }
}

std::string kudroma::code_assistant::PackageParser::parseItemName(const std::string& line)
{
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(" "));
    if (parts.size() >= 2)
        return parts[1];
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" invalid package line '%2%'") % __FUNCTION__ % line);

    return std::string();
}

bool kudroma::code_assistant::PackageParser::createPackageItem(const std::string& name)
{
    return false;
}

uint8_t kudroma::code_assistant::PackageParser::parseHierarchyLevel(const std::string& line)
{
    uint8_t level{ 0 };
    const auto size = line.size();
    for (auto i = 0; i < size; ++i)
    {
        if (line[i] == ' ')
            ++level;
    }
    return level;
}