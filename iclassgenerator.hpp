#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace kudroma { namespace code_assistant {

class IClassGenerator
{
public:
    static const std::uint_fast8_t HEADER{ 0b0000'0001 };
    static const std::uint_fast8_t SOURCE{ 0b0000'0010 };
    static const std::uint_fast8_t EMPTY_FILE{ 0b0000'0100 };
    static const std::uint_fast8_t CLASS{ 0b0000'1000 };

    bool generate(const fs::path& dir, std::string& name, const std::uint_fast8_t& properties = EMPTY_FILE);

protected:
    virtual bool generateClass(const fs::path& dir, const std::string& classname) { return false; };
    virtual bool generateHeader(const fs::path& dir, const std::string& classname) { return false; };
    virtual bool generateSource(const fs::path& dir, const std::string& sourcename) { return false; };
    virtual bool generateEmptyFile(const fs::path& dir, const std::string& filename);
};

}}