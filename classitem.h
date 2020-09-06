#ifndef CLASSITEM_H
#define CLASSITEM_H

#include "item.h"

namespace kudroma {
namespace code_assistant {

class ClassItem : public Item
{
public:
    ClassItem(const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent);
};
}}

#endif // CLASSITEM_H
