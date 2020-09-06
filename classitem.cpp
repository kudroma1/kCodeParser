#include "classitem.h"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

using namespace kudroma::code_assistant;

kudroma::code_assistant::ClassItem::ClassItem(const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent) : Item(dir, name, parent)
{
    type_ = ItemType::Class;
}