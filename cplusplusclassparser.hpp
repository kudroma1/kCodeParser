#pragma once

#include "iclassparser.h"

namespace kudroma{namespace code_parser{

class CplusplusClassParser : public IClassParser
{
public:
    std::shared_ptr<ClassDesc> tryParse(const std::string& line) override;
};

}}