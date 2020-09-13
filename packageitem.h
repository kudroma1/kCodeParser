#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H

#include "item.h"

namespace kudroma {
namespace code_assistant {

class PackageItem : public Item
{
public:
    PackageItem(const std::string& name, const std::shared_ptr<Item> parent);

    void add(std::shared_ptr<Item> item) override;

    bool build() override;
};
}}

#endif // PACKAGEITEM_H
