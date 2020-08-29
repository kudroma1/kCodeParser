#include "itemfactory.h"

#include "packageitem.h"
#include "classitem.h"

using namespace kudroma::code_assistant;

std::string ItemFactory::package_{ "pk" };
std::string ItemFactory::class_{ "cl" };

std::shared_ptr<Item> kudroma::code_assistant::ItemFactory::createItem(const std::string& item, const QDir& dir, const std::string& name)
{
    if (item == package_)
        return std::make_shared<PackageItem>(dir, name);
    if (item == class_)
        return std::make_shared<ClassItem>(dir, name);
    else
        return nullptr;
}