#include "packageitem.h"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

using namespace kudroma::code_assistant;

PackageItem::PackageItem(const std::string& name, const std::shared_ptr<Item> parent) : Item(name, parent)
{
    type_ = ItemType::Package;
}

void kudroma::code_assistant::PackageItem::add(std::shared_ptr<Item> item)
{
    if (item)
    {
        auto childPath = dir_ / name_;
        item->setDir(childPath);
        Item::add(item);
    }
}

bool kudroma::code_assistant::PackageItem::build()
{
    if (fs::exists(dir_))
    {
        auto new_dir = dir_ / name_;
        if (!fs::exists(new_dir))
        {
            if (!fs::create_directory(new_dir))
            {
                BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << "not able to create directory " << new_dir;
                return false;
            }
        }
    }

    for (auto& child : children_)
    {
        if (!child->build())
            return false;
    }

    return true;
}
