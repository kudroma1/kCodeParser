#ifndef ITEM_H
#define ITEM_H

#include "lang.h"

#include <QtCore/QDir>
#include <QtCore/QSharedPointer>

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
        Item(const QDir& dir, const std::string& name);

        void addItem(const QSharedPointer<Item> item);

        virtual void action(QSharedPointer<Lang> lang) = 0;

        ItemType type() const { return type_; }

    protected:
        QList<QSharedPointer<Item> > childItems_;
        QDir dir_;
        std::string name_;
        ItemType type_{ ItemType::Item };
    };
}}

#endif // ITEM_H
