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

#include "opcpackagepart.h"
#include "opcpackagepart_p.h"
#include "opcpackage.h"
#include "opcpackagerelationship_p.h"
#include "opcutils_p.h"

namespace QtOfficeOpenXml {
namespace Opc {

/*!
 * \class QtOfficeOpenXml::Opc::PackagePartPrivate
 * \internal
 */

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

/*!
 * \class QtOfficeOpenXml::Opc::PackagePart
 * \inmodule QtOfficeOpenXml
 */

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
