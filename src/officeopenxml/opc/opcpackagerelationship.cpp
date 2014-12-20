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

#include "opcpackagerelationship.h"
#include "opcpackagerelationship_p.h"
#include "opcutils_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"

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

PackageRelationshipHelper::PackageRelationshipHelper(Package *package, const QString &sourcePartName)
    :m_package(package), m_relationshipsPart(0), m_sourcePartName(sourcePartName)
{
    if (package->mode() & QIODevice::ReadOnly) {
        //Try Load from the part stream.
        m_relationshipsPart = m_package->part(getRelsPath(m_sourcePartName));
        if (m_relationshipsPart) {
            doLoadFromXml(m_relationshipsPart->getDevice());
            m_relationshipsPart->releaseDevice();
        }
    }
}

PackageRelationshipHelper::~PackageRelationshipHelper()
{
    qDeleteAll(m_relationships);
}

void PackageRelationshipHelper::flush()
{
    if (m_package->mode() == QIODevice::WriteOnly) {
        if (!m_relationshipsPart) {
            m_relationshipsPart = m_package->createPart(getRelsPath(m_sourcePartName), QStringLiteral("application/vnd.openxmlformats-package.relationships+xml"));
            //relationshp part can not have relationships with other parts.
        }
        doSaveToXml(m_relationshipsPart->getDevice());
        m_relationshipsPart->releaseDevice();
    }
}

PackageRelationship *PackageRelationshipHelper::relationship(const QString &id) const
{
    if (m_relationships.contains(id))
        return m_relationships[id];
    return 0;
}

QList<PackageRelationship *> PackageRelationshipHelper::relationships() const
{
    return m_relationships.values();
}

QList<PackageRelationship *> PackageRelationshipHelper::getRelationshipsByType(const QString &type) const
{
    QList<PackageRelationship *> rels;
    QMapIterator<QString, PackageRelationship * > it(m_relationships);
    while (it.hasNext()) {
        it.next();
        if (it.value()->relationshipType() == type)
            rels.append(it.value());
    }
    return rels;
}

PackageRelationship *PackageRelationshipHelper::createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id)
{
    //If id already in-used, return.
    if (!id.isEmpty() && m_relationships.contains(id))
        return 0;

    QString realId = id;
    //Generated a properly one.
    if (realId.isEmpty()) {
        int idx = m_relationships.size();
        do {
            ++idx;
            realId = QStringLiteral("rId%1").arg(idx);
        } while (m_relationships.contains(realId));
    }

    PackageRelationship *rel = new PackageRelationship(realId, type, m_sourcePartName, target, mode);
    m_relationships.insert(realId, rel);
    return rel;
}

void PackageRelationshipHelper::deleteRelationship(const QString &id)
{
    m_relationships.remove(id);
}

void PackageRelationshipHelper::doLoadFromXml(QIODevice *device)
{
    m_relationships.clear();
    QXmlStreamReader reader(device);
    while(!reader.atEnd()) {
        if (reader.readNextStartElement() && reader.name() == QLatin1String("Relationship")) {
            QXmlStreamAttributes attributes = reader.attributes();
            const QString id = attributes.value(QLatin1String("Id")).toString();
            const QString type = attributes.value(QLatin1String("Type")).toString();
            QString target = attributes.value(QLatin1String("Target")).toString();
            const TargetMode targetMode = attributes.value(QLatin1String("TargetMode")) == QLatin1String("External")
                    ? External : Internal;
            if (targetMode == Internal)
                target = getAbsolutePartName(m_sourcePartName, target);
            m_relationships.insert(id, new PackageRelationship(id, type, m_sourcePartName, target, targetMode));
        }
    }
}

void PackageRelationshipHelper::doSaveToXml(QIODevice *device)
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Relationships"));
    writer.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships"));
    foreach (PackageRelationship *relation, m_relationships) {
        writer.writeStartElement(QStringLiteral("Relationship"));
        writer.writeAttribute(QStringLiteral("Id"), relation->id());
        writer.writeAttribute(QStringLiteral("Type"), relation->relationshipType());
        if (relation->targetMode() == External) {
            writer.writeAttribute(QStringLiteral("Target"), relation->target());
            writer.writeAttribute(QStringLiteral("TargetMode"), QStringLiteral("External"));
        } else {
            writer.writeAttribute(QStringLiteral("Target"), getRelativePartName(m_sourcePartName, relation->target()));
        }
        writer.writeEndElement();
    }
    writer.writeEndElement();//Relationships
    writer.writeEndDocument();
}

} // namespace Opc
} // namespace QtOfficeOpenXml
