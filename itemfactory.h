#pragma once

#include <QtCore/QDir>

#include <memory>
#include <string>

namespace kudroma { namespace code_assistant {

    class Item;
    class ItemFactory
    {
    public:
        static std::shared_ptr<Item> createItem(const std::string& item, const QDir& dir, const std::string& name);

    private:
        static std::string package_;
        static std::string class_;
    };

}}