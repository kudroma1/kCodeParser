#pragma once

#include "iclassgenerator.hpp"

#include <set>

namespace kudroma { namespace code_assistant{

    class ClassGeneratorFactory
    {
    public:
        static std::shared_ptr<IClassGenerator> generateClassGenerator(const std::string& lang);

    private:
        static std::set<std::string> cplusplusPattern_;
};

}}