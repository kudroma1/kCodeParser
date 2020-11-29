#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H

#include "item.h"

namespace kudroma {
namespace code_parser {

    class PackageDesc;

class PackageItem : public Item
{
public:
    PackageItem(const std::shared_ptr<PackageDesc> desc, const std::shared_ptr<Item> parent);

    void add(std::shared_ptr<Item> item) override;

    bool build() override;
};
}}

#endif // PACKAGEITEM_H
