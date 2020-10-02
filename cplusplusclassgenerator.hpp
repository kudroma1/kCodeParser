#pragma once

#include "iclassgenerator.hpp"

#include <boost/foreach.hpp>
#include <boost/log/trivial.hpp>

namespace kudroma { namespace code_assistant {

class CplusplusClassGenerator : public IClassGenerator
{
public:
    CplusplusClassGenerator();

protected:
    virtual bool generateClass(const fs::path& dir, const std::string& classname, const std::vector<std::string>& namespaces = std::vector<std::string>()) override;
    virtual bool generateHeader(const fs::path& dir, const std::string& classname, const std::vector<std::string>& namespaces = std::vector<std::string>()) override;
    virtual bool generateSource(const fs::path& dir, const std::string& sourcename, const std::vector<std::string>& namespaces = std::vector<std::string>()) override;

private:
    std::string headerTemplate_;
    const std::string cHeaderTemplateFilename_ = "resources/templates/class/cplusplus_header.txt";

    std::string sourceTemplate_;
    const std::string cSourceTemplateFilename_ = "resources/templates/class/cplusplus_source.txt";
};

}}