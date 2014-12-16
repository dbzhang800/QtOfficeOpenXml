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

#include "opcpackagerelationship.h"
#include "opcpackagerelationship_p.h"
#include "opcutils_p.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace QtOfficeOpenXml {
namespace Opc {

/*!
 * \class PackageRelationship
 *
 * Represents an association between a source Package or PackagePart, and a target
 * object which can be a PackagePart or external resource.
 */

PackageRelationship::PackageRelationship(const QString &id, const QString &type, const QString &source, const QString &target, TargetMode targetMode)
    :d_ptr(new PackageRelationshipPrivate(id, type, source, target, targetMode))
{
}

PackageRelationship::~PackageRelationship()
{
    delete d_ptr;
}

QString PackageRelationship::id() const
{
    return d_func()->id;
}

QString PackageRelationship::relationshipType() const
{
    return d_func()->relationshipType;
}

QString PackageRelationship::source() const
{
    return d_func()->source;
}

QString PackageRelationship::target() const
{
    return d_func()->target;
}

TargetMode PackageRelationship::targetMode() const
{
    return d_func()->targetMode;
}


QMap<QString, PackageRelationship*> PackageRelationshipHelper::loadRelationshipFromStream(QIODevice *device, const QString &source)
{
    QMap<QString, PackageRelationship*> relationships;
    QXmlStreamReader reader(device);
    while(!reader.atEnd()) {
        if (reader.readNextStartElement() && reader.name() == QLatin1String("Relationship")) {
            QXmlStreamAttributes attributes = reader.attributes();
            const QString id = attributes.value(QLatin1String("Id")).toString();
            const QString type = attributes.value(QLatin1String("Type")).toString();
            const QString target = attributes.value(QLatin1String("Target")).toString();
            const TargetMode targetMode = attributes.value(QLatin1String("TargetMode")) == QLatin1String("External")
                    ? External : Internal;
            relationships.insert(id.toUpper(), new PackageRelationship(id, type, source, getAbsolutePartName(source, target), targetMode));
        }
    }

    return relationships;
}

void PackageRelationshipHelper::saveRelationshipToStream(QIODevice *device, const QList<PackageRelationship *> &relationshipList)
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Relationships"));
    writer.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships"));
    foreach (PackageRelationship *relation, relationshipList) {
        writer.writeStartElement(QStringLiteral("Relationship"));
        writer.writeAttribute(QStringLiteral("Id"), relation->id());
        writer.writeAttribute(QStringLiteral("Type"), relation->relationshipType());
        writer.writeAttribute(QStringLiteral("Target"), getRelativePartName(relation->source(), relation->target()));
        if (relation->targetMode() == External)
            writer.writeAttribute(QStringLiteral("TargetMode"), QStringLiteral("External"));
        writer.writeEndElement();
    }
    writer.writeEndElement();//Relationships
    writer.writeEndDocument();
}

} // namespace Opc
} // namespace QtOfficeOpenXml
