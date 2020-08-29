#include "classitem.h"

using namespace kudroma::code_assistant;

kudroma::code_assistant::ClassItem::ClassItem(const QDir& dir, const std::string& name) : Item(dir, name)
{
    type_ = ItemType::Class;
}

void kudroma::code_assistant::ClassItem::action(QSharedPointer<Lang> lang)
{
}