#ifndef KUDROMA_CODE_ASSISTANT_LANGITEM_H
#define KUDROMA_CODE_ASSISTANT_LANGITEM_H

#include <QtCore/QDir>
#include <QtCore/QString>

namespace kudroma {
namespace code_assistant {

class Lang
{
public:
    Lang();

    virtual void createClass(const QDir& dir, const QString& className) = 0;
    virtual void createPackage(const QDir& dir, const QString& packageName) = 0;

protected:
    void createFile(const QDir& dir, const QString& filename) const;
    void createDir(const QDir& dir, const QString& dirname) const;
};

} // namespace code_assistant
} // namespace kudroma

#endif // KUDROMA_CODE_ASSISTANT_LANGITEM_H
