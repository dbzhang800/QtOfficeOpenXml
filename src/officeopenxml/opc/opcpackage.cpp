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
#include "opcpackage.h"
#include "opcpackage_p.h"
#include "opcpackagepart.h"
#include "opcpackagerelationship.h"
#include "opcpartbasedpackageproperties_p.h"

#include <QFile>

namespace QtOfficeOpenXml {
namespace Opc {

PackagePrivate::PackagePrivate(const QString &packageName, QIODevice *device, Package *q)
    :q_ptr(q), fileName(packageName), device(device), mode(QIODevice::NotOpen),
      packageProperties(0)
{

}

PackagePrivate::~PackagePrivate()
{

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
    qDeleteAll(d->relationships);
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
    if (d->relationships.contains(id.toUpper()))
        return d->relationships[id.toUpper()];
    return 0;
}

QList<PackageRelationship *> Package::relationships() const
{
    Q_D(const Package);
    return d->relationships.values();
}

QList<PackageRelationship *> Package::getRelationshipsByType(const QString &type) const
{
    Q_D(const Package);
    QList<PackageRelationship *> rels;
    QMapIterator<QString, PackageRelationship * > it(d->relationships);
    while (it.hasNext()) {
        it.next();
        if (it.value()->relationshipType().toUpper() == type.toUpper())
            rels.append(it.value());
    }
    return rels;
}

PackageRelationship *Package::createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id)
{
    Q_D(Package);
    //If id already in-used, return.
    if (!id.isEmpty() && relationship(id))
        return 0;

    QString realId = id;
    //Generated a properly one.
    if (realId.isEmpty()) {
        int idx = d->relationships.size();
        do {
            ++idx;
            realId = QStringLiteral("rId%1").arg(idx);
        } while (relationship(realId));
    }

    PackageRelationship *rel = new PackageRelationship(realId, type, QStringLiteral("/"), target, mode);
    d->relationships.insert(realId.toUpper(), rel);
    return rel;
}

void Package::deleteRelationship(const QString &id)
{
    Q_D(Package);
    delete relationship(id);
    d->relationships.remove(id.toUpper());
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

} // namespace Opc
} // namespace QtOfficeOpenXml
