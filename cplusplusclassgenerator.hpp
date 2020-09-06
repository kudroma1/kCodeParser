#pragma once

#include "iclassgenerator.hpp"

#include <boost/foreach.hpp>
#include <boost/log/trivial.hpp>

namespace kudroma { namespace code_assistant {

class CplusplusClassGenerator : public IClassGenerator
{
protected:
    virtual bool generateClass(const fs::path& dir, const std::string& classname) override;
    virtual bool generateHeader(const fs::path& dir, const std::string& classname) override;
    virtual bool generateSource(const fs::path& dir, const std::string& sourcename) override;
};

}}