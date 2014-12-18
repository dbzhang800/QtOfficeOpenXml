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

#include "opcpackagepart.h"
#include "opcpackagepart_p.h"

namespace QtOfficeOpenXml {
namespace Opc {

PackagePartPrivate::PackagePartPrivate(const QString &partName, const QString type, PackagePart *q, Package *package)
    :q_ptr(q), package(package), partName(partName), contentType(type)
{

}

PackagePartPrivate::~PackagePartPrivate()
{

}

PackagePart::PackagePart(PackagePartPrivate *d)
    :d_ptr(d)
{

}

PackagePart::~PackagePart()
{
    delete d_ptr;
}

QString PackagePart::partName() const
{
    Q_D(const PackagePart);
    return d->partName;
}

QString PackagePart::contentType() const
{
    Q_D(const PackagePart);
    return d->contentType;
}

QIODevice *PackagePart::getDevice()
{
    return doGetDevice();
}

void PackagePart::releaseDevice()
{
    doReleaseDevice();
}

PackageRelationship *PackagePart::relationship(const QString &id) const
{
    Q_D(const PackagePart);
    if (d->relationships.contains(id.toUpper()))
        return d->relationships[id.toUpper()];
    return 0;
}

QList<PackageRelationship *> PackagePart::relationships() const
{
    Q_D(const PackagePart);
    return d->relationships.values();
}

QList<PackageRelationship *> PackagePart::getRelationshipsByType(const QString &type) const
{
    Q_D(const PackagePart);
    QList<PackageRelationship *> rels;
    QMapIterator<QString, PackageRelationship*> it(d->relationships);
    while (it.hasNext()) {
        it.next();
        if (it.value()->relationshipType().toUpper() == type.toUpper())
            rels.append(it.value());
    }
    return rels;
}

PackageRelationship *PackagePart::createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id)
{
    Q_D(PackagePart);
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

void PackagePart::deleteRelationship(const QString &id)
{
    Q_D(PackagePart);
    delete relationship(id);
    d->relationships.remove(id.toUpper());
}

} // namespace Opc
} // namespace QtOfficeOpenXml
