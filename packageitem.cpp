#include "packageitem.h"

using namespace kudroma::code_assistant;

PackageItem::PackageItem(const QDir &dir, const std::string& name) : Item(dir, name)
{
    type_ = ItemType::Package;
}

void PackageItem::action(QSharedPointer<Lang> lang)
{
   // if(lang)
   //     lang->createPackage(dir_, name_);
}
