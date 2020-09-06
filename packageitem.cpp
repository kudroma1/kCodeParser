#include "packageitem.h"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

using namespace kudroma::code_assistant;

PackageItem::PackageItem(const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent) : Item(dir, name, parent)
{
    type_ = ItemType::Package;
}

void kudroma::code_assistant::PackageItem::addItem(std::shared_ptr<Item> item)
{
    auto childPath = dirPath_;
    childPath += fs::path(name_);
    item->setDirPath(childPath);
    Item::addItem(item);
}