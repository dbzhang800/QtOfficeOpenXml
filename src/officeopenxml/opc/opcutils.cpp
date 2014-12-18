/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang
** Contact: hello@debao.me
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#include "opcutils_p.h"
#include <QDir>
#include <QStringList>

namespace QtOfficeOpenXml {
namespace Opc {

QStringList splitPartName(const QString &partName)
{
    int idx = partName.lastIndexOf(QLatin1Char('/'));
    if (idx == -1)
        return QStringList()<<QStringLiteral(".")<<partName;

    if (idx == 0)
        return QStringList()<<partName<<QString();

    return QStringList()<<partName.left(idx)<<partName.mid(idx+1);
}

QString getAbsolutePartName(const QString &baseName, const QString &relativeName)
{
    QString path = QDir(splitPartName(baseName)[0]).absoluteFilePath(relativeName);
    return QDir::cleanPath(path);
}

QString getRelativePartName(const QString &baseName, const QString &absoluteName)
{
    QString path = QDir(splitPartName(baseName)[0]).relativeFilePath(absoluteName);
    return QDir::cleanPath(path);
}

/*
 * Return the .rel file path based on partName
 */
QString getRelsPath(const QString &partName)
{
    int idx = partName.lastIndexOf(QLatin1Char('/'));
    if (idx == -1)
        return QString();

    return QString(partName.left(idx) + QLatin1String("/_rels/")
                   + partName.mid(idx+1) + QLatin1String(".rels"));
}

} // namespace Opc
} // namespace QtOfficeOpenXml
