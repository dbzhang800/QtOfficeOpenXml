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
#ifndef OOXMLPACKAGE_H
#define OOXMLPACKAGE_H

#include <QtOfficeOpenXml/opcpackagerelationship.h>
#include <QtCore/qiodevice.h>

namespace QtOfficeOpenXml {
namespace Opc {

class PackagePart;
class PackageRelationship;
class PackageProperties;
class PackagePrivate;
class Q_OFFICEOPENXML_EXPORT Package
{
    Q_DECLARE_PRIVATE(Package)
public:
    virtual ~Package();
    bool open(QIODevice::OpenMode mode);
    bool close();
    bool isOpen() const;
    QIODevice::OpenMode mode() const;
    QIODevice *device() const;
    QString packageName() const;

    static Package *open(QIODevice *device, QIODevice::OpenMode mode);
    static Package *open(const QString &packageName, QIODevice::OpenMode mode);

    PackagePart *part(const QString &partName) const;
    QList<PackagePart *> parts() const;
    PackagePart *createPart(const QString &partName, const QString &contentType);
    void deletePart(const QString &partName);

    PackageRelationship *relationship(const QString &id) const;
    QList<PackageRelationship *> relationships() const;
    QList<PackageRelationship *> getRelationshipsByType(const QString &type) const;
    PackageRelationship *createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id=QString());
    void deleteRelationship(const QString &id);

    PackageProperties *packageProperties() const;

protected:
    Package(PackagePrivate *d);
    virtual bool doOpenPackage(QIODevice::OpenMode mode) = 0;
    virtual bool doClosePackage() = 0;
    virtual PackagePart *doCreatePart(const QString &partName, const QString &contentType) = 0;
    virtual bool doDeletePart(const QString &partName) = 0;

    PackagePrivate *d_ptr;
};

} // namespace Opc
} // namespace QtOfficeOpenXml
#endif // OOXMLPACKAGE_H
