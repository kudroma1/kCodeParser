#ifndef CLASSITEM_H
#define CLASSITEM_H

#include "item.h"

namespace kudroma {
namespace code_parser {

class IClassGenerator;
class ClassDesc;

class ClassItem : public Item
{
public:
    ClassItem(const std::shared_ptr<ClassDesc> desc, const std::shared_ptr<Item> parent);

    bool build() override;

private:
    std::shared_ptr<IClassGenerator> classGenerator_{ nullptr };
    std::shared_ptr<ClassDesc> desc_{ nullptr };
};
}}

#endif // CLASSITEM_H
