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
#include "opcpackage.h"
#include "opcpackage_p.h"
#include "opcpackagepart_p.h"
#include "opcpackagerelationship_p.h"
#include "opcpartbasedpackageproperties_p.h"
#include "opczippackage.h"

#include <QFile>

namespace QtOfficeOpenXml {
namespace Opc {

PackagePrivate::PackagePrivate(const QString &packageName, QIODevice *device, Package *q)
    :q_ptr(q), fileName(packageName), device(device), mode(QIODevice::NotOpen),
     relationshipHelper(0), packageProperties(0)
{

}

PackagePrivate::~PackagePrivate()
{
}

void PackagePrivate::ensureRelationship() const
{
    if (!relationshipHelper) {
        PackagePrivate *self = const_cast<PackagePrivate *>(this);
        self->relationshipHelper = new PackageRelationshipHelper(self->q_func(), QStringLiteral("/"));
    }
}

/*!
 * \class Package
 *
 *  Package is an abstract class that can be used to organize objects into a
 *  single entity of a defined physical format for portability and efficient access.
 *  A ZIP file is the primary physical format for the Package.
 */

Package::Package(PackagePrivate *d)
    :d_ptr(d)
{
}

Package::~Package()
{
    Q_D(Package);
    qDeleteAll(d->parts);
    if (d->relationshipHelper)
        delete d->relationshipHelper;
    if (d->packageProperties)
        delete d->packageProperties;

    delete d;
}

bool Package::open(QIODevice::OpenMode mode)
{
    Q_D(Package);
    Q_ASSERT(mode != QIODevice::NotOpen);

    if (isOpen())
        close();

    bool ret = doOpenPackage(mode);
    if (ret)
        d->mode = mode;

    return ret;
}

bool Package::close()
{
    Q_D(Package);
    if (!isOpen())
        return true;

    if (d->packageProperties)
        d->packageProperties->flush();

    foreach (PackagePart *part, d->parts)
        part->d_func()->flushRelationships();

    //Other operation such as packagePropertiesPart
    //add root relationship item. So flush it after
    //all others have been flushed.
    if (d->relationshipHelper)
        d->relationshipHelper->flush();

    doClosePackage();

    d->mode = QIODevice::NotOpen;
    return true;
}

bool Package::isOpen() const
{
    Q_D(const Package);
    return d->mode != QIODevice::NotOpen;
}

QIODevice::OpenMode Package::mode() const
{
    Q_D(const Package);
    return d->mode;
}

QIODevice *Package::device() const
{
    Q_D(const Package);
    return d->device;
}

QString Package::packageName() const
{
    Q_D(const Package);
    return d->fileName;
}

PackagePart *Package::part(const QString &partName) const
{
    Q_D(const Package);
    if (d->parts.contains(partName.toUpper()))
        return d->parts[partName.toUpper()];
    return 0;
}

QList<PackagePart *> Package::parts() const
{
    Q_D(const Package);
    return d->parts.values();
}

PackagePart *Package::createPart(const QString &partName, const QString &contentType)
{
    Q_D(Package);
    if (part(partName))
        return 0;
    PackagePart *p = doCreatePart(partName, contentType);
    d->parts.insert(partName.toUpper(), p);
    return p;
}

void Package::deletePart(const QString &partName)
{
    Q_D(Package);
    if (!part(partName))
        return;
    doDeletePart(partName);
    d->parts.remove(partName.toUpper());
}

PackageRelationship *Package::relationship(const QString &id) const
{
    Q_D(const Package);
    d->ensureRelationship();
    return d->relationshipHelper->relationship(id);
}

QList<PackageRelationship *> Package::relationships() const
{
    Q_D(const Package);
    d->ensureRelationship();
    return d->relationshipHelper->relationships();
}

QList<PackageRelationship *> Package::getRelationshipsByType(const QString &type) const
{
    Q_D(const Package);
    d->ensureRelationship();
    return d->relationshipHelper->getRelationshipsByType(type);
}

PackageRelationship *Package::createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id)
{
    Q_D(Package);
    d->ensureRelationship();
    return d->relationshipHelper->createRelationship(target, mode, type, id);
}

void Package::deleteRelationship(const QString &id)
{
    Q_D(Package);
    d->ensureRelationship();
    d->relationshipHelper->deleteRelationship(id);
}

PackageProperties *Package::packageProperties() const
{
    Q_D(const Package);
    if (!d->packageProperties) {
        Package *self = const_cast<Package *>(this);
        self->d_func()->packageProperties = new PartBasedPackageProperties(self);
    }
    return d->packageProperties;
}

/*!
 * ZipPackage is the default package type that is used by the open method.
*/
Package * Package::open(QIODevice *device, QIODevice::OpenMode mode)
{
    Package *package = new ZipPackage(device);
    package->open(mode);
    return package;
}

/*!
 * ZipPackage is the default package type that is used by the open method.
*/
Package * Package::open(const QString &packageName, QIODevice::OpenMode mode)
{
    Package *package = new ZipPackage(packageName);
    package->open(mode);
    return package;
}

} // namespace Opc
} // namespace QtOfficeOpenXml
