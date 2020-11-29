#pragma once

#include "ClassDesc.h"

#include <boost/regex.hpp>

#include <cstdint>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace kudroma { namespace code_parser {

class IClassGenerator
{
public:
    static const std::uint_fast8_t HEADER{ 0b0000'0001 };
    static const std::uint_fast8_t SOURCE{ 0b0000'0010 };
    static const std::uint_fast8_t EMPTY_FILE{ 0b0000'0100 };
    static const std::uint_fast8_t CLASS{ 0b0000'1000 };

    bool generate(const fs::path& dir, std::shared_ptr<ClassDesc> desc, const std::uint_fast8_t& properties);

protected:
    virtual bool generateClass(const fs::path&, const std::shared_ptr<ClassDesc>) = 0;
    virtual bool generateHeader(const fs::path&, const std::shared_ptr<ClassDesc>) = 0;
    virtual bool generateSource(const fs::path& dir, const std::shared_ptr<ClassDesc>) = 0;
    virtual bool generateEmptyFile(const fs::path& dir, const std::string& filename);

protected:
    static boost::regex TAG_CLASS_HEADER;
    static boost::regex TAG_CLASS_NAME;
    static boost::regex TAG_CLASS_NAMESPACE;
    static boost::regex TAG_CLASS_NAMESPACE_START;
    static boost::regex TAG_CLASS_NAMESPACE_END;
    static boost::regex TAG_CLASS_PIMPL;
    static boost::regex TAG_CLASS_CTOR;
    static boost::regex TAG_CLASS_DTOR;
    static boost::regex TAG_CLASS_VDTOR;
    static boost::regex TAG_CLASS_MOVE_CTOR;
    static boost::regex TAG_CLASS_COPY_CTOR;
    static boost::regex TAG_CLASS_OP_COPY_ASIGN;
    static boost::regex TAG_CLASS_OP_MOVE_ASIGN;
    static boost::regex TAG_CLASS_INCLUDES;
    static boost::regex TAG_SEP;
};

}}