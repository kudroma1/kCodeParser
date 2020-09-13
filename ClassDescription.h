#pragma once

#include <string>
#include <vector>

namespace kudroma { namespace code_assistant{ 

    struct VarDesc
    {
        std::string type;
        std::string name;
        std::vector<std::string> qualifiers;
        std::string defaultValue;
    };

class ClassDescription
{
private:
    std::vector<std::string> namespaces_;
};

}}