#pragma once

#include "item.h"

#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace kudroma { namespace code_assistant {

    class ItemFactory
    {
    public:
        static std::shared_ptr<Item> createItem(const ItemType& type, const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent);

    private:
        static std::string package_;
        static std::string class_;
    };

}}