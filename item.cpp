#include "item.h"

using namespace kudroma::code_assistant;

Item::Item(const fs::path& dirPath, const std::string& name, const std::shared_ptr<Item> parent) : dirPath_(dirPath), name_(name), parent_(parent)
{
}

void Item::addItem(std::shared_ptr<Item> item)
{
    if(item)
        children_.push_back(item);
}
