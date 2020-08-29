#include "cplusplus.h"

using namespace kudroma::code_assistant;

Cplusplus::Cplusplus()
{
}

void Cplusplus::createClass(const QDir &dir, const QString &className)
{
    createFile(dir, QString("%1.hpp").arg(className));
    createFile(dir, QString("%1.cpp").arg(className));
}

void Cplusplus::createPackage(const QDir &dir, const QString &packageName)
{
}
