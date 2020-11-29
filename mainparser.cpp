#include "mainparser.hpp"

#include "classdesc.h"
#include "classitem.h"
#include "classutilityfactory.hpp"
#include "packagedesc.hpp"
#include "iclassparser.h"
#include "ipackageparser.hpp"
#include "packageitem.h"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include <fstream>

using namespace kudroma::code_parser;

bool MainParser::parseProjectTree(const std::string& filename, const fs::path& dir)
{
    if (!fs::exists(dir))
    {
        BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% dir \"%2%\": doesn't exist") % __FUNCTION__ % dir);
        return false;
    }
    std::ifstream file;
    file.open(filename);
    if (file.is_open())
    {
        std::string line;
        uint32_t lineNum{ 1 };
        dir_ = dir;
        clear();

        // check lang
        std::getline(file, line);
        if(!parseLang(line))
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("Line%1% in \"%2%\": \"%1%\" doesn't contain lang") % line % filename);
            return false;
        }

        // check content
        while (!file.eof())
        {
            lineNum++;
            std::getline(file, line);
            if (!parseLine(line))
                break;
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

bool kudroma::code_parser::MainParser::buildProjectTree()
{
    if (root_)
        return root_->build();
    else
    {
        BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " root item is empty";
        return false;
    }
}

void kudroma::code_parser::MainParser::clear()
{
    spaceNum_ = 0;
    parentItem_ = nullptr;
    lastItem_ = nullptr;
    root_ = nullptr;
    curHierarchyLevel_ = 0;
    lang_ = "";
    namespaces_.clear();
}

bool kudroma::code_parser::MainParser::parseLine(const std::string& line)
{
    const auto level = parseHierarchyLevel(line);
    if (auto classDesc = classParser_->tryParse(line); classDesc)
    {
        classDesc->lang = lang_;
        return handleClass(level, classDesc);
    }
    else if (auto packageDesc = packageParser_->tryParse(line); packageDesc)
    {
        if (!handlePackage(level, packageDesc))
            return false;
        else
        {
            if (!root_)
            {
                parentItem_ = lastItem_;
                root_ = lastItem_;
                root_->setDir(dir_);
            }
            return true;
        }
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Didn't recognize line " << line;
        return false;
    }
}

bool kudroma::code_parser::MainParser::parseLang(const std::string& line)
{
    if (boost::regex_match(line, langRegex_))
    {
        std::vector<std::string> parts;
        boost::split(parts, line, boost::is_any_of(" "));
        lang_ = parts.back();
        if (classParser_ = ClassUtilityFactory::generateClassParser(lang_); !classParser_)
        {
            BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Class parser for was not created for lang=" << lang_;
            return false;
        }
        if (packageParser_ = ClassUtilityFactory::generatePackageParser(lang_); !packageParser_)
        {
            BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Class parser for was not created for lang=" << lang_;
            return false;
        }
        BOOST_LOG_TRIVIAL(debug) << "Lang : " << lang_;
        return true;
    }
    else
        return false;
}

uint8_t kudroma::code_parser::MainParser::parseHierarchyLevel(const std::string& line)
{
    uint8_t level{ 0 };
    const auto size = line.size();
    for (auto i = 0; i < size; ++i)
    {
        if (line[i] == ' ')
            ++level;
        else
            break;
    }

    if (level % tabSize_)
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line '%1%' is not aligned by spaces"));
        return -1;
    }
    else
        return level / tabSize_;
}

bool kudroma::code_parser::MainParser::handleClass(uint8_t level, const std::shared_ptr<ClassDesc> desc)
{
    if (!handleHierarchyLevel(level))
        return false;

    if(lastItem_ = std::make_shared<ClassItem>(desc, parentItem_); lastItem_)
    {
        if (parentItem_)
            parentItem_->add(lastItem_);
        curHierarchyLevel_ = level;
        return true;
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Class item was not created.";
        return false;
    }
}

bool kudroma::code_parser::MainParser::handlePackage(uint8_t level, const std::shared_ptr<PackageDesc> desc)
{
    if (!handleHierarchyLevel(level))
        return false;

    if(lastItem_ = std::make_shared<PackageItem>(desc, parentItem_); lastItem_)
    {
        if (parentItem_)
            parentItem_->add(lastItem_);
        if (lastItem_->type() == ItemType::Package)
            parentItem_ = lastItem_;
        curHierarchyLevel_ = level;
        return true;
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " Package item was not created.";
        return false;
    }
}

bool kudroma::code_parser::MainParser::handleHierarchyLevel(uint8_t level)
{
    // incorrect level handling
    if (level < 0)
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " negative level=" << level;
        return false;
    }
    // go to previous hierarchy level
    else if (level < curHierarchyLevel_)
    {
        if (parentItem_ && !parentItem_->parent().expired())
            parentItem_ = parentItem_->parent().lock();
        else
            parentItem_ = nullptr;
    }

    return true;
}
