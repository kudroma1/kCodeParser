#include "item.h"

using namespace kudroma::code_assistant;

Item::Item(const std::string& name, const std::shared_ptr<Item> parent) : name_(name), parent_(parent)
{
}

void Item::add(std::shared_ptr<Item> item)
{
    if(item)
        children_.push_back(item);
}
