/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang <hello@debao.me>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Library General Public License version
** 2.1 or version 3 as published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public License
** along with this library; see the file LICENSE.LGPLv21 and LICENSE.LGPLv3
** included in the packaging of this file. If not, write to the Free
** Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
** 02110-1301, USA.
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
