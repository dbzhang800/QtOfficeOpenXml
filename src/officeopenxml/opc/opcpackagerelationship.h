/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
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
#ifndef QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H
#define QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H

#include <QtOfficeOpenXml/officeopenxmlglobal.h>
#include <QtCore/qstring.h>

namespace QtOfficeOpenXml {
namespace Opc {

enum TargetMode
{
    External,
    Internal
};

class PackageRelationshipPrivate;
class Q_OFFICEOPENXML_EXPORT PackageRelationship
{
    Q_DECLARE_PRIVATE(PackageRelationship)
public:
    PackageRelationship(const QString &id, const QString &type, const QString &source, const QString &target, TargetMode targetMode=Internal);
    ~PackageRelationship();

    QString id() const;
    QString relationshipType() const;
    QString source() const;
    QString target() const;
    TargetMode targetMode() const;

private:
    PackageRelationshipPrivate *d_ptr;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H
