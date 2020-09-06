#ifndef ITEM_H
#define ITEM_H

#include <filesystem>
#include <memory>
#include <vector>

namespace fs = std::filesystem;

namespace kudroma {
namespace code_assistant {

    enum class ItemType {
        Item,
        Class,
        Package
    };

    class Item
    {
    public:
        Item(const fs::path& dir, const std::string& name, const std::shared_ptr<Item> parent);

        virtual void addItem(std::shared_ptr<Item> item);
        void setDirPath(const fs::path& path) { dirPath_ = path; }

        ItemType type() const { return type_; }

        std::weak_ptr<Item> parent() const { return parent_; }

    protected:
        std::vector<std::shared_ptr<Item> > children_;
        fs::path dirPath_;
        std::string name_;
        ItemType type_{ ItemType::Item };
        std::weak_ptr<Item> parent_;
    };
}}

#endif // ITEM_H
