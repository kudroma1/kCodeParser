#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H

#include "item.h"

namespace kudroma {
namespace code_assistant {

class PackageItem : public Item
{
public:
    PackageItem(const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent);

    void addItem(std::shared_ptr<Item> item) override;
};
}}

#endif // PACKAGEITEM_H
