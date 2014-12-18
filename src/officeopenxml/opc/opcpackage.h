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
#ifndef OOXMLPACKAGE_H
#define OOXMLPACKAGE_H

#include "ooxmlglobal.h"
#include "opcpackagerelationship.h"
#include <QIODevice>

class QIODevice;

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
