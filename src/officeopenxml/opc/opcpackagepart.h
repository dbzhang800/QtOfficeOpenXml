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
#ifndef QTOFFICEOPENXML_OPC_OPCPACKAGEPART_H
#define QTOFFICEOPENXML_OPC_OPCPACKAGEPART_H

#include "ooxmlglobal.h"
#include "opcpackagerelationship.h"

class QIODevice;

namespace QtOfficeOpenXml {
namespace Opc {
class Package;
class PackagePartPrivate;
class Q_OFFICEOPENXML_EXPORT PackagePart
{
    Q_DECLARE_PRIVATE(PackagePart)
public:
    virtual ~PackagePart();
    QString partName() const;
    QString contentType() const;

    QIODevice *getDevice();
    void releaseDevice();

    PackageRelationship *relationship(const QString &id) const;
    QList<PackageRelationship *> relationships() const;
    QList<PackageRelationship *> getRelationshipsByType(const QString &type) const;
    PackageRelationship *createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id=QString());
    void deleteRelationship(const QString &id);

protected:
    friend class Package;

    PackagePart(PackagePartPrivate *d);
    virtual QIODevice *doGetDevice() = 0;
    virtual void doReleaseDevice() = 0;

    PackagePartPrivate *d_ptr;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCPACKAGEPART_H
