#ifndef CLASSITEM_H
#define CLASSITEM_H

#include "item.h"

namespace kudroma {
namespace code_assistant {

class IClassGenerator;

class ClassItem : public Item
{
public:
    ClassItem(const std::string& name, const std::string& lang, const std::shared_ptr<Item> parent);

    bool build() override;

private:
    std::shared_ptr<IClassGenerator> classGenerator_{ nullptr };
};
}}

#endif // CLASSITEM_H
