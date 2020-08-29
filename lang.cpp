#include "lang.h"

using namespace kudroma::code_assistant;

Lang::Lang()
{}

void Lang::createFile(const QDir &dir, const QString &filename) const
{
    QString name = QString("%1/%2.txt").arg(dir.absolutePath()).arg(filename);
    QFile file;
    file.setFileName(name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.close();
}

void Lang::createDir(const QDir &dir, const QString &dirname) const
{
    dir.mkdir(dirname);
}
