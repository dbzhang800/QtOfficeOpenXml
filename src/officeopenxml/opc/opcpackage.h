/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
**
** This file is part of the QtOfficeOpenXml library.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPLv2 included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
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
    PackageRelationship *getRelationshipByType(const QString &type) const;
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
