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

bool PackageParser::parseProjectTree(const std::string& filename, const fs::path& dir)
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
        spaceNum_ = 0;
        parentItem_ = nullptr;
        lastItem_ = nullptr;
        rootItem_ = nullptr;
        curHierarchyLevel_ = 0;
        dir_ = dir;
        lang_ = "";
        namespaces_.clear();

        // check lang
        std::getline(file, line);
        if (boost::regex_match(line, langRegex_))
        {
            std::vector<std::string> parts;
            boost::split(parts, line, boost::is_any_of(" "));
            lang_ = parts.back();
            BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Lang : %1%") % lang_);
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

bool kudroma::code_assistant::PackageParser::buildProjectTree()
{
    if (rootItem_)
        return rootItem_->build();
    else
    {
        BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " root item is empty";
    }
}

bool kudroma::code_assistant::PackageParser::parseLine(const std::string& line)
{
    // Handle namespaces
    if (boost::regex_match(line, namespaceRegex_))
    {
        std::vector<std::string> namespaces;
        boost::split(namespaces, line, boost::is_any_of(" "));
        for (int i = namespaces.size() - 1; i >= 0; --i)
        {
            if (!namespaces[i].empty())
            {
                boost::split(namespaces_, namespaces[i], boost::is_any_of(":"));
                break;
            }
        }
        return true;
    }

    // Handle top-level package
    if (!parentItem_ && boost::regex_match(line, packageRegex_))
    {
        curHierarchyLevel_ = parseHierarchyLevel(line);
        parentItem_ = createItem(line, nullptr);
        lastItem_ = parentItem_;
        if (!lastItem_)
        {
            BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" doesn't contain package name") % line);
            return false;
        }
        else
        {
            rootItem_ = lastItem_;
            lastItem_->setDir(dir_);
            return true;
        }
    }

    const auto level = parseHierarchyLevel(line);
    // incorrect level handling
    if (level < 0)
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%2% Line \"%1%\" has bad hierarchy level") % line % __FUNCTION__);
        return false;
    }
    // go to previous hierarchy level
    else if (level < curHierarchyLevel_)
    {
        if (parentItem_ && !parentItem_->parent().expired())
            parentItem_ = parentItem_->parent().lock();
        else
        {
            BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%2% Line \"%1%\": Finish parsing successfully.") % line % __FUNCTION__);
            parentItem_ = nullptr;
            return true;
        }
    }

    lastItem_ = createItem(line, parentItem_);
    if(parentItem_)
        parentItem_->add(lastItem_);
    if (lastItem_)
    {
        if (lastItem_->type() == ItemType::Package)
            parentItem_ = lastItem_;
        curHierarchyLevel_ = level;
        return true;
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%2% Line \"%1%\": item was not created.") % line % __FUNCTION__);
        return false;
    }
}

std::string kudroma::code_assistant::PackageParser::parseItemName(const std::string& line)
{
    auto local = line;
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(" "));
    std::vector<std::string> words;
    words.reserve(parts.size());
    for (auto&& p : parts)
    {
        if (!p.empty())
            words.emplace_back(std::move(p));
    }
    if (words.size() >= 2)
        return words[1];
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" invalid package line '%2%'") % __FUNCTION__ % line);

    return std::string();
}

std::shared_ptr<Item> kudroma::code_assistant::PackageParser::createItem(const std::string& line, const std::shared_ptr<Item> parent)
{
    if (boost::regex_match(line, packageRegex_))
    {
        const auto name = parseItemName(line);
        return std::make_shared<PackageItem>(name, parent);
    }
    else if (boost::regex_match(line, classRegex_) 
        || boost::regex_match(line, classWithInheritanceRegex_))
    {
        const auto name = parseItemName(line);
        return std::make_shared<ClassItem>(name, lang_, parent, namespaces_);
    }
    else
        return nullptr;
}

uint8_t kudroma::code_assistant::PackageParser::parseHierarchyLevel(const std::string& line)
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