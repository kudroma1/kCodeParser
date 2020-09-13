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
            auto h = boost::str(boost::format(headerTemplate_) % classname);
            header.write(h.c_str(), h.size() - 12);
            header.close();
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
            source << boost::str(boost::format(sourceTemplate_) % headerName);
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% source file '%2%' already exists in '%3%'") % __FUNCTION__ % sourceName % dir);

    return true;
}
