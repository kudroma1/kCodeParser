#pragma once

#include <bitset>
#include <string>
#include <unordered_set>
#include <vector>

namespace kudroma{ namespace code_parser{ 

class ClassDesc
{
public:
    std::string lang = "";
    std::string name = "";
    std::string parent = "";
    std::unordered_set<std::string> props;
};

class CplusplusClassDesc : public ClassDesc
{
public:
    static const std::uint_fast8_t COPY_CTOR{ 0b0000'0001 };
    static const std::uint_fast8_t MOVE_CTOR{ 0b0000'0010 };
    static const std::uint_fast8_t MOVE_ASSIGN_OP{ 0b0000'0100 };
    static const std::uint_fast8_t COPY_ASSIGN_OP{ 0b0000'1000 };

    std::vector<std::string> namespaces;
    bool pimple{ false };
    std::uint_fast8_t special_member_functions;
};

}}