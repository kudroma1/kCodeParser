#include "item.h"

using namespace kudroma::code_parser;

kudroma::code_parser::Item::Item(const std::shared_ptr<Item> parent)
{
    parent_ = parent;
}

Item::Item(const std::string& name, const std::shared_ptr<Item> parent) : name_(name), parent_(parent)
{
}

void Item::add(std::shared_ptr<Item> item)
{
    if(item)
        children_.push_back(item);
}
