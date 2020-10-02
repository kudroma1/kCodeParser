#include "cplusplusclassgenerator.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

#include <fstream>

kudroma::code_assistant::CplusplusClassGenerator::CplusplusClassGenerator()
{
    {
        std::fstream file;
        file.open(cHeaderTemplateFilename_, std::ios_base::in);
        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            const auto size = file.tellg();
            headerTemplate_.resize(size);
            file.seekg(0, std::ios::beg);
            file.read(&headerTemplate_[0], size);
            int size1 = headerTemplate_.size();
            file.close();
        }
        else
            BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " Couldn't open header template " << cHeaderTemplateFilename_;
    }

    {
        std::fstream file;
        file.open(cSourceTemplateFilename_, std::ios_base::in);
        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            const auto size = file.tellg();
            sourceTemplate_.resize(size);
            file.seekg(0, std::ios::beg);
            file.read(&sourceTemplate_[0], size);
            int size1 = sourceTemplate_.size();
            file.close();
        }
        else
            BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " Couldn't open source template " << cSourceTemplateFilename_;
    }
}

bool kudroma::code_assistant::CplusplusClassGenerator::generateClass(const fs::path& dir, const std::string& classname, const std::vector<std::string>& namespaces)
{
    return generateHeader(dir, classname, namespaces) && generateSource(dir, classname, namespaces);
}

bool kudroma::code_assistant::CplusplusClassGenerator::generateHeader(const fs::path& dir, const std::string& classname, const std::vector<std::string>& namespaces)
{
    std::fstream header;
    auto local = classname;
    boost::to_lower(local);
    const auto headerName = local + ".hpp";
    auto headerPath = dir / headerName;
    if (!fs::exists(dir))
    {
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% dir '%2%' doesn't exist") % __FUNCTION__ % dir);
        return false;
    }

    if (!fs::exists(headerPath))
    {
        header.open(headerPath, std::ios_base::out);
        if (!header.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% header file '%2%' was not created in '%3%") % __FUNCTION__ % headerName % dir);
            return false;
        }
        else
        {
            std::string namespacesStartStr, namespacesEndStr;
            for (const auto& n : namespaces)
            {
                namespacesStartStr += boost::str(boost::format("namespace %1%{") % n);
                namespacesEndStr += "}";
            }
            auto h = boost::str(boost::format(headerTemplate_) % namespacesStartStr % classname % namespacesEndStr);
            header.write(h.c_str(), h.size() - 16);
            header.close();
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% header file '%2%' already exists in '%3%'") % __FUNCTION__ % headerName % dir);

    return true;
}

bool kudroma::code_assistant::CplusplusClassGenerator::generateSource(const fs::path& dir, const std::string& sourcename, const std::vector<std::string>& namespaces)
{
    std::fstream source;
    auto local = sourcename;
    boost::to_lower(local);
    const auto sourceName = local + ".cpp";
    const auto headerName = local + ".hpp";
    auto sourcePath = dir / sourceName;
    if (!fs::exists(sourcePath))
    {
        source.open(sourcePath, std::ios_base::out);
        if (!source.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% source file '%2%' was not created in '%3%") % __FUNCTION__ % sourceName % dir);
            return false;
        }
        else
        {
            std::string namespaceStr;
            for (const auto& n : namespaces)
                namespaceStr += boost::str(boost::format("%1%::") % n);
            
            if (!namespaceStr.empty())
                namespaceStr.resize(namespaceStr.size() - 2);
            std::string s = boost::str(boost::format(sourceTemplate_) % headerName % namespaceStr);
            if (!s.empty())
                s.resize(s.size() - 2);
            source << s;
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% source file '%2%' already exists in '%3%'") % __FUNCTION__ % sourceName % dir);

    return true;
}
