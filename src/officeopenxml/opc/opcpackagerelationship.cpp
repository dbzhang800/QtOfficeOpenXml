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

#include "opcpackagerelationship.h"
#include "opcpackagerelationship_p.h"
#include "opcutils_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"

#include "mcexmlstreamreader.h"
#include <QXmlStreamWriter>
#include <QDebug>

namespace QtOfficeOpenXml {
namespace Opc {

/*!
  \enum QtOfficeOpenXml::Opc::TargetMode

  \value Internal
  \value External

 */

/*!
 * \class QtOfficeOpenXml::Opc::PackageRelationship
 * \inmodule QtOfficeOpenXml
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

/*!
 * \class QtOfficeOpenXml::Opc::PackageRelationshipPrivate
 * \internal
 */

/*!
 * \class QtOfficeOpenXml::Opc::PackageRelationshipHelper
 * \internal
 */
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
            m_relationshipsPart = m_package->createPart(getRelsPath(m_sourcePartName), QString::fromLatin1(ContentTypes::relationships));
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
    Mce::XmlStreamReader reader(device);
    reader.addMceUnderstoodNamespace(QString::fromLatin1(NamespaceIds::relationships));
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

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

    if (reader.hasError() && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        qWarning()<<reader.errorString();
}

void PackageRelationshipHelper::doSaveToXml(QIODevice *device)
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Relationships"));
    writer.writeDefaultNamespace(QString::fromLatin1(NamespaceIds::relationships));
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
