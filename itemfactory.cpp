#include "itemfactory.h"

#include "packageitem.h"
#include "classitem.h"

using namespace kudroma::code_assistant;

std::shared_ptr<Item> kudroma::code_assistant::ItemFactory::createItem(const ItemType& type, const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent)
{
    if (type == ItemType::Package)
        return std::make_shared<PackageItem>(dir, name, parent);
    if (type == ItemType::Class)
        return std::make_shared<ClassItem>(dir, name, parent);
    else
        return nullptr;
}