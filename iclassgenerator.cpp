#include "iclassgenerator.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

#include <fstream>

using namespace kudroma::code_parser;

boost::regex IClassGenerator::TAG_CLASS_HEADER          = boost::regex("%class_header%");
boost::regex IClassGenerator::TAG_CLASS_NAME            = boost::regex("%class_name%");
boost::regex IClassGenerator::TAG_CLASS_NAMESPACE       = boost::regex("%class_namespace%");
boost::regex IClassGenerator::TAG_CLASS_NAMESPACE_START = boost::regex("%class_namespace_start%");
boost::regex IClassGenerator::TAG_CLASS_NAMESPACE_END   = boost::regex("%class_namespace_end%");
boost::regex IClassGenerator::TAG_CLASS_PIMPL           = boost::regex("%class_pimpl%");
boost::regex IClassGenerator::TAG_CLASS_CTOR            = boost::regex("%class_default_ctor%");
boost::regex IClassGenerator::TAG_CLASS_DTOR            = boost::regex("%class_default_dtor%");
boost::regex IClassGenerator::TAG_CLASS_VDTOR           = boost::regex("%class_default_vdtor%");
boost::regex IClassGenerator::TAG_CLASS_MOVE_CTOR       = boost::regex("%class_move_ctor%");
boost::regex IClassGenerator::TAG_CLASS_COPY_CTOR       = boost::regex("%class_copy_ctor%");
boost::regex IClassGenerator::TAG_CLASS_OP_COPY_ASIGN   = boost::regex("%class_op_copy_asign%");
boost::regex IClassGenerator::TAG_CLASS_OP_MOVE_ASIGN   = boost::regex("%class_op_move_asign%");
boost::regex IClassGenerator::TAG_CLASS_INCLUDES        = boost::regex("%class_includes%");
boost::regex IClassGenerator::TAG_SEP                   = boost::regex("%sep%");

bool IClassGenerator::generate(const fs::path& dir, std::shared_ptr<ClassDesc> desc, const std::uint_fast8_t& properties)
{
    if (!desc)
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " ClassDesc is nullptr";
        return false;
    }

    if (properties & CLASS)
        return generateClass(dir, desc);
    if (properties & HEADER && properties & SOURCE)
        return generateSource(dir, desc) && generateHeader(dir, desc);
    if(properties & HEADER)
        return generateHeader(dir, desc);
    if (properties & SOURCE)
        return generateSource(dir, desc);
    if (properties & EMPTY_FILE)
        return generateEmptyFile(dir, desc->name);

    // generates nothing
    return false;
}

bool kudroma::code_parser::IClassGenerator::generateEmptyFile(const fs::path& dir, const std::string& filename)
{
    std::fstream file;
    auto temp = filename;
    boost::to_lower(temp);
    auto sourcePath = dir;
    if (!fs::exists(sourcePath))
    {
        file.open(temp, std::ios_base::out);
        if (!file.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("%1% empty file '%2%' was not created in '%3%") % __FUNCTION__ % temp % dir);
            return false;
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% empty file '%2%' already exists in '%3%'") % __FUNCTION__ % temp % dir);

    return true;
}
