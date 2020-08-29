#ifndef CPLUSPLUS_H
#define CPLUSPLUS_H

#include "lang.h"

#include <QtCore/QDir>

namespace kudroma { namespace code_assistant {

class Cplusplus : public Lang
{
public:
    Cplusplus();

    void createClass(const QDir& dir, const QString& className) override;
    void createPackage(const QDir& dir, const QString& packageName) override;
};
}}

#endif // CPLUSPLUS_H
