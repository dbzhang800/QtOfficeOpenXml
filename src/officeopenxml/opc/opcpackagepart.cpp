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
