#include "cplusplusclassgenerator.hpp"
#include "ClassDesc.h"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include <fstream>
#include <typeinfo>
#include <type_traits>

kudroma::code_parser::CplusplusClassGenerator::CplusplusClassGenerator()
{
    {
        std::ifstream file;
        file.open(cHeaderTemplateFilename_, std::ios_base::in);
        if (file.is_open())
        {
            std::string local;
            while (!file.eof())
            {
                std::getline(file, local);
                headerTemplate_ += local += "\n";
            }
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
            std::string local;
            while (!file.eof())
            {
                std::getline(file, local);
                sourceTemplate_ += local += "\n";
            }
            file.close();
        }
        else
            BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " Couldn't open source template " << cSourceTemplateFilename_;
    }
}

bool kudroma::code_parser::CplusplusClassGenerator::generateClass(const fs::path& dir, const std::shared_ptr<ClassDesc> desc)
{
    return generateHeader(dir, desc) && generateSource(dir, desc);
}

bool kudroma::code_parser::CplusplusClassGenerator::generateHeader(const fs::path& dir, const std::shared_ptr<ClassDesc> desc)
{
    if (!desc)
    {
        BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " ClassDesc is nullptr.";
        return false;
    }
    const auto name = typeid(desc.get()).name();

    std::fstream header;
    auto local = desc->name;
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
            for (const auto& n : std::static_pointer_cast<CplusplusClassDesc>(desc)->namespaces)
            {
                namespacesStartStr += boost::str(boost::format("namespace %1% { ") % n);
                namespacesEndStr += "}";
            }

            std::string includesStr;
            std::string pimplStr;
            std::string ctorStr;
            std::string dtorStr;
            std::string vdtorStr;
            std::string copyCtorStr;
            std::string moveCtorStr;
            std::string opMoveAsignStr;
            std::string opCopyAsignStr;
            if (desc->props.find("c") != desc->props.cend())
            {
                ctorStr = "%sep%" + desc->name + "();";
            }
            if (desc->props.find("d") != desc->props.cend())
            {
                dtorStr = "%sep%~" + desc->name + "();";
            }
            if (desc->props.find("vd") != desc->props.cend())
            {
                vdtorStr = "%sep%virtual ~" + desc->name + "();";
            }
            if (desc->props.find("=") != desc->props.cend())
            {
                opCopyAsignStr = "%sep%" + desc->name + "& opearator=(const " + desc->name + "& other);";
            }
            if (desc->props.find("=&&") != desc->props.cend())
            {
                opMoveAsignStr = "%sep%" + desc->name + "& opearator=(" + desc->name + "&& other);";
            }
            if (desc->props.find("&") != desc->props.cend())
            {
                copyCtorStr = "%sep%" + desc->name + "& " + desc->name + "(const " + desc->name + "& other);";
            }
            if (desc->props.find("&&") != desc->props.cend())
            {
                moveCtorStr = "%sep%" + desc->name + "& " + desc->name + "(" + desc->name + "&& other);";
            }
            if (desc->props.find("pimpl") != desc->props.cend())
            {
                pimplStr = "%sep%struct Impl;\n";
                pimplStr += "%sep%std::experimental::propagate_const<std::unique_ptr<Impl>> pImpl_{ nullptr; }";
                includesStr += "#include <memory>;\n#include <experimental/propagate_const>;\n";
            }

            auto result = headerTemplate_;
            result = boost::regex_replace(result, TAG_CLASS_INCLUDES,        includesStr);
            result = boost::regex_replace(result, TAG_CLASS_NAME,            desc->name);
            result = boost::regex_replace(result, TAG_CLASS_NAMESPACE_START, namespacesStartStr);
            result = boost::regex_replace(result, TAG_CLASS_NAMESPACE_END,   namespacesEndStr);
            result = boost::regex_replace(result, TAG_CLASS_PIMPL,           pimplStr);
            result = boost::regex_replace(result, TAG_CLASS_CTOR,            ctorStr);
            result = boost::regex_replace(result, TAG_CLASS_DTOR,            dtorStr);
            result = boost::regex_replace(result, TAG_CLASS_VDTOR,           vdtorStr);
            result = boost::regex_replace(result, TAG_CLASS_COPY_CTOR,       copyCtorStr);
            result = boost::regex_replace(result, TAG_CLASS_MOVE_CTOR,       moveCtorStr);
            result = boost::regex_replace(result, TAG_CLASS_OP_COPY_ASIGN,   opCopyAsignStr);
            result = boost::regex_replace(result, TAG_CLASS_OP_MOVE_ASIGN,   opMoveAsignStr);

            result = boost::regex_replace(result, boost::regex("\\n\\s*\\n"), "\n\n");
            result = boost::regex_replace(result, TAG_SEP, sep_);
            header << result;
            header.close();
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% header file '%2%' already exists in '%3%'") % __FUNCTION__ % headerName % dir);
    return true;
}

bool kudroma::code_parser::CplusplusClassGenerator::generateSource(const fs::path& dir, const std::shared_ptr<ClassDesc> desc)
{
    std::fstream source;
    auto className = desc->name;
    boost::to_lower(className);
    const auto sourceName = className + ".cpp";
    const auto headerName = className + ".hpp";
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
            std::string namespaceStr("");
            for (const auto& n : std::static_pointer_cast<CplusplusClassDesc>(desc)->namespaces)
                 namespaceStr += boost::str(boost::format("%1%::") % n);
            
            if (!namespaceStr.empty())
                namespaceStr.resize(namespaceStr.size() - 2);

            const bool hasPimpl = desc->props.find("pimpl") != desc->props.cend();
            
            std::string pimplStr;
            std::string ctorStr;
            std::string dtorStr;
            std::string vdtorStr;
            std::string copyCtorStr;
            std::string moveCtorStr;
            std::string opMoveAsignStr;
            std::string opCopyAsignStr;
            if (hasPimpl)
            {
                pimplStr = "struct " + desc->name + "::Impl\n{\n};";
            }
            if (desc->props.find("c") != desc->props.cend())
            {
                ctorStr = desc->name + "::" + desc->name + "()";
                if (!desc->parent.empty())
                    ctorStr += " : " + desc->parent + "()";
                ctorStr += "\n{\n";
                if (hasPimpl)
                    ctorStr += "%sep%impl_ = std::make_unique<Impl>();\n";
                ctorStr += "}\n";
            }
            if (desc->props.find("d") != desc->props.cend())
            {
                dtorStr = desc->name + "::~" + desc->name + "()\n{}";
            }
            if (desc->props.find("vd") != desc->props.cend())
            {
                vdtorStr = desc->name + "::~" + desc->name + "()\n{}";
            }
            if (desc->props.find("=") != desc->props.cend())
            {
                opCopyAsignStr = desc->name + "& " + desc->name + "::opearator=(const " + desc->name + "& other)\n{\n%sep%return *this;\n}";
            }
            if (desc->props.find("=&&") != desc->props.cend())
            {
                opMoveAsignStr = desc->name + "& " + desc->name + "::opearator=(" + desc->name + "&& other)\n{\n%sep%return *this;\n}";
            }
            if (desc->props.find("&") != desc->props.cend())
            {
                copyCtorStr = desc->name + "& " + desc->name + "::" + desc->name + "(const " + desc->name + "& other) \n{\n%sep%return *this;\n}";
            }
            if (desc->props.find("&&") != desc->props.cend())
            {
                moveCtorStr = desc->name + "& " + desc->name + "::" + desc->name + "(" + desc->name + "&& other) \n{\n%sep%return *this;\n}";
            }

            auto result = sourceTemplate_;
            result = boost::regex_replace(result, TAG_CLASS_HEADER,        headerName);
            result = boost::regex_replace(result, TAG_CLASS_NAMESPACE,     namespaceStr);
            result = boost::regex_replace(result, TAG_CLASS_CTOR,          ctorStr);
            result = boost::regex_replace(result, TAG_CLASS_DTOR,          dtorStr);
            result = boost::regex_replace(result, TAG_CLASS_VDTOR,         vdtorStr);
            result = boost::regex_replace(result, TAG_CLASS_COPY_CTOR,     copyCtorStr);
            result = boost::regex_replace(result, TAG_CLASS_MOVE_CTOR,     moveCtorStr);
            result = boost::regex_replace(result, TAG_CLASS_OP_COPY_ASIGN, opCopyAsignStr);
            result = boost::regex_replace(result, TAG_CLASS_OP_MOVE_ASIGN, opMoveAsignStr);
            result = boost::regex_replace(result, TAG_CLASS_PIMPL,         pimplStr);
            
            result = boost::regex_replace(result, boost::regex("\\n\\s*\\n"), "\\n\\n");
            result = boost::regex_replace(result, TAG_SEP, sep_);
            source << result;
            source.close();
        }
    }
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("%1% source file '%2%' already exists in '%3%'") % __FUNCTION__ % sourceName % dir);
    
    return true;
}