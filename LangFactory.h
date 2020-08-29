#pragma once

#include <memory>
#include <string>

namespace kudroma { namespace code_assistant {

    class Lang;

    class LangFactory
    {
    public:
        static std::shared_ptr<Lang> createLang(const std::string& lang);

    private:
        static std::string cplusplus_;
    };

}}