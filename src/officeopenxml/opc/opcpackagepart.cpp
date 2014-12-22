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
#include "opcpackage.h"
#include "opcpackagerelationship_p.h"
#include "opcutils_p.h"

namespace QtOfficeOpenXml {
namespace Opc {

PackagePartPrivate::PackagePartPrivate(const QString &partName, const QString type, PackagePart *q, Package *package)
    :q_ptr(q), package(package), partName(partName), contentType(type), relationshipHelper(0)
{

}

PackagePartPrivate::~PackagePartPrivate()
{
    if (relationshipHelper)
        delete relationshipHelper;
}

void PackagePartPrivate::ensureRelationship() const
{
    if (!relationshipHelper) {
        PackagePartPrivate *self = const_cast<PackagePartPrivate *>(this);
        self->relationshipHelper = new PackageRelationshipHelper(self->package, self->partName);
    }
}

bool PackagePartPrivate::isRelationshipPart() const
{
    return contentType == QLatin1String(ContentTypes::relationships);
}

void PackagePartPrivate::flushRelationships()
{
    if (!relationshipHelper || relationshipHelper->relationships().isEmpty())
        return;

    relationshipHelper->flush();
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
    //RelationshipPart can not have relationship with another parts.
    if (d->isRelationshipPart())
        return 0;
    d->ensureRelationship();
    return d->relationshipHelper->relationship(id);
}

QList<PackageRelationship *> PackagePart::relationships() const
{
    Q_D(const PackagePart);
    if (d->isRelationshipPart())
        return QList<PackageRelationship *>();

    d->ensureRelationship();
    return d->relationshipHelper->relationships();
}

QList<PackageRelationship *> PackagePart::getRelationshipsByType(const QString &type) const
{
    Q_D(const PackagePart);
    if (d->isRelationshipPart())
        return QList<PackageRelationship *>();
    d->ensureRelationship();
    return d->relationshipHelper->getRelationshipsByType(type);
}

PackageRelationship *PackagePart::createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id)
{
    Q_D(PackagePart);
    if (d->isRelationshipPart())
        return 0;
    d->ensureRelationship();
    return d->relationshipHelper->createRelationship(target, mode, type, id);
}

void PackagePart::deleteRelationship(const QString &id)
{
    Q_D(PackagePart);
    if (d->isRelationshipPart())
        return;
    d->ensureRelationship();
    d->relationshipHelper->deleteRelationship(id);
}

} // namespace Opc
} // namespace QtOfficeOpenXml
