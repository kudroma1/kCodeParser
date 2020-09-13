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

    class Item : std::enable_shared_from_this<Item>
    {
    public:
        Item(const std::string& name, const std::shared_ptr<Item> parent);

        virtual void add(std::shared_ptr<Item> item);
        void setDir(const fs::path& path) { dir_ = path; }

        ItemType type() const { return type_; }

        std::weak_ptr<Item> parent() const { return parent_; }

        virtual bool build() = 0;

    protected:
        std::vector<std::shared_ptr<Item> > children_;
        fs::path dir_;
        std::string name_;
        ItemType type_{ ItemType::Item };
        std::weak_ptr<Item> parent_;
    };
}}

#endif // ITEM_H
