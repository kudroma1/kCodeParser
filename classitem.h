#ifndef CLASSITEM_H
#define CLASSITEM_H

#include "item.h"

namespace kudroma {
namespace code_assistant {

class IClassGenerator;

class ClassItem : public Item
{
public:
    ClassItem(const std::string& name, const std::string& lang, const std::shared_ptr<Item> parent, std::vector<std::string> namespaces = std::vector<std::string>());

    bool build() override;

private:
    std::shared_ptr<IClassGenerator> classGenerator_{ nullptr };
    std::vector<std::string> namespaces_;
};
}}

#endif // CLASSITEM_H
