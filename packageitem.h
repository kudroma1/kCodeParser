#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H

#include "item.h"

namespace kudroma {
namespace code_assistant {

class PackageItem : public Item
{
public:
    PackageItem(const QDir& dir, const std::string& name);

    void action(QSharedPointer<Lang> lang) override;
};
}}

#endif // PACKAGEITEM_H
