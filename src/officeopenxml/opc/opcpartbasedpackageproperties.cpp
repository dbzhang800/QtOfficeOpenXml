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

#include "opcpartbasedpackageproperties_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"
#include "opcutils_p.h"
#include "mcexmlstreamreader.h"

#include <QDateTime>
#include <QXmlStreamWriter>
#include <QDebug>

namespace QtOfficeOpenXml {
namespace Opc {

static const char * propertiesNameTable[] = {
    //Note, keep the save order as PropertyEnum.
    "creator",
    "identifier",
    "title",
    "subject",
    "description",
    "language",
    "created",
    "modified",
    "contentType",
    "keywords",
    "category",
    "version",
    "lastModifiedBy",
    "contentStatus",
    "revision",
    "lastPrinted"
};

/*!
 * \class QtOfficeOpenXml::Opc::PartBasedPackageProperties
 * \internal
 */

PartBasedPackageProperties::PartBasedPackageProperties(Package *package)
    :m_package(package), m_propertyPart(0)
{
    if (m_package->mode() & QIODevice::ReadOnly)
        readPropertiesFromPackage();
}

PartBasedPackageProperties::~PartBasedPackageProperties()
{
}

void PartBasedPackageProperties::flush()
{
    if (m_package->mode() == QIODevice::WriteOnly) {
        if (!m_propertyPart) {
            //create part if not exists.
            m_propertyPart = m_package->createPart(QStringLiteral("/docProps/core.xml"), QString::fromLatin1(ContentTypes::coreProperties));
            m_package->createRelationship(m_propertyPart->partName(), Internal, QString::fromLatin1(RelationshipTypes::coreProperties));
        }

        QIODevice *stream = m_propertyPart->getDevice();
        doSaveToXml(stream);
        m_propertyPart->releaseDevice();
    }
}

void PartBasedPackageProperties::readPropertiesFromPackage()
{
    QList<PackageRelationship *> relationships = m_package->getRelationshipsByType(QString::fromLatin1(RelationshipTypes::coreProperties));
    if (relationships.size() != 1)
        return;
    m_propertyPart = m_package->part(relationships[0]->target());
    if (!m_propertyPart)
        return;
    QIODevice *corePropertyStream = m_propertyPart->getDevice();
    doLoadFromXml(corePropertyStream);
    m_propertyPart->releaseDevice();
}

void PartBasedPackageProperties::doLoadFromXml(QIODevice *device)
{
    Mce::XmlStreamReader reader(device);
    reader.setMceUnderstoodNamespaces(QSet<QString>()
                                      << QString::fromLatin1(NamespaceIds::coreProperties)
                                      << QString::fromLatin1(NamespaceIds::dc)
                                      << QString::fromLatin1(NamespaceIds::dcterms)
                                      << QString::fromLatin1(NamespaceIds::dcmitype)
                                      << QString::fromLatin1(NamespaceIds::xsi));
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    while (!reader.atEnd()) {
         if (reader.readNextStartElement()) {
             const QStringRef name = reader.name();
             //Todo, "keywords" support lang attribute.
             for (int idx=0; idx<16; ++idx) {
                 if (name == QLatin1String(propertiesNameTable[idx])) {
                     m_properties[static_cast<PropertyEnum>(idx)] = reader.readElementText();
                     break;
                 }
             }
         }
    }
    if (reader.hasError() && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        qWarning()<<reader.errorString();
}

void PartBasedPackageProperties::doSaveToXml(QIODevice *device)
{
    QXmlStreamWriter writer(device);
    const QString cp = QString::fromLatin1(NamespaceIds::coreProperties);
    const QString dc = QString::fromLatin1(NamespaceIds::dc);
    const QString dcterms = QString::fromLatin1(NamespaceIds::dcterms);
    const QString dcmitype = QString::fromLatin1(NamespaceIds::dcmitype);
    const QString xsi = QString::fromLatin1(NamespaceIds::xsi);
    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("cp:coreProperties"));
    writer.writeNamespace(cp, QStringLiteral("cp"));
    writer.writeNamespace(dc, QStringLiteral("dc"));
    writer.writeNamespace(dcterms, QStringLiteral("dcterms"));
    writer.writeNamespace(dcmitype, QStringLiteral("dcmitype"));
    writer.writeNamespace(xsi, QStringLiteral("xsi"));

    QMapIterator<PropertyEnum, QString> it(m_properties);
    while(it.hasNext()) {
        it.next();
        PropertyEnum pe = it.key();
        switch (pe) {
        case PE_Category:
        case PE_ContentStatus:
        case PE_Keywords:
        case PE_LastModifiedBy:
        case PE_LastPrinted:
        case PE_Revision:
            writer.writeTextElement(cp, QString::fromLatin1(propertiesNameTable[pe]), m_properties[pe]);
            break;
        case PE_Creator:
        case PE_Description:
        case PE_Identifier:
        case PE_Language:
        case PE_Subject:
        case PE_Title:
        case PE_Version:
            writer.writeTextElement(dc, QString::fromLatin1(propertiesNameTable[pe]), m_properties[pe]);
            break;
        case PE_Created:
        case PE_Modified:
            writer.writeStartElement(dcterms, QString::fromLatin1(propertiesNameTable[pe]));
            writer.writeAttribute(xsi, QStringLiteral("type"), QStringLiteral("dcterms:W3CDTF"));
            writer.writeCharacters(m_properties[pe]);
            writer.writeEndElement();
            break;
        }
    }

    writer.writeEndElement(); //cp:coreProperties
    writer.writeEndDocument();
}

QString PartBasedPackageProperties::stringProperty(PropertyEnum pe) const
{
    if (m_properties.contains(pe))
        return m_properties[pe];
    return QString();
}

QDateTime PartBasedPackageProperties::dateTimeProperty(PropertyEnum pe) const
{
    if (!m_properties.contains(pe))
        return QDateTime();
    QString dt_string = m_properties[pe];

    //TODO
    QDateTime dt = QDateTime::fromString(dt_string, Qt::ISODate);
    if (dt.isNull())
        dt = QDateTime::fromString(dt_string, Qt::TextDate);
    if (dt.isNull())
        dt = QDateTime::fromString(dt_string, Qt::RFC2822Date);
    if (dt.isNull())
        dt = QDateTime::fromString(dt_string, Qt::SystemLocaleLongDate);
    if (dt.isNull())
        dt = QDateTime::fromString(dt_string, Qt::SystemLocaleShortDate);
    return dt;
}

void PartBasedPackageProperties::setStringProperty(PropertyEnum pe, const QString &value)
{
    if (value.isNull())
        m_properties.remove(pe);
    else
        m_properties[pe] = value;
}

void PartBasedPackageProperties::setDateTimeProperty(PropertyEnum pe, const QDateTime &value)
{
    if (value.isNull())
        m_properties.remove(pe);
    else
        m_properties[pe] = value.toString(Qt::ISODate);
}

//Summary information properties
QString PartBasedPackageProperties::title() const
{
    return stringProperty(PE_Title);
}

void PartBasedPackageProperties::setTitle(const QString &title)
{
    setStringProperty(PE_Title, title);
}

QString PartBasedPackageProperties::subject() const
{
    return stringProperty(PE_Subject);
}

void PartBasedPackageProperties::setSubject(const QString &subject)
{
    setStringProperty(PE_Subject, subject);
}

QString PartBasedPackageProperties::creator() const
{
    return stringProperty(PE_Creator);
}

void PartBasedPackageProperties::setCreator(const QString &creator)
{
    setStringProperty(PE_Creator, creator);
}

QString PartBasedPackageProperties::keywords()const
{
    return stringProperty(PE_Keywords);
}

void PartBasedPackageProperties::setKeywords(const QString &keywords)
{
    setStringProperty(PE_Keywords, keywords);
}

QString PartBasedPackageProperties::description()const
{
    return stringProperty(PE_Description);
}

void PartBasedPackageProperties::setDescription(const QString &description)
{
    setStringProperty(PE_Description, description);
}

QString PartBasedPackageProperties::lastModifiedBy() const
{
    return stringProperty(PE_LastModifiedBy);
}

void PartBasedPackageProperties::setLastModifiedBy(const QString &user)
{
    setStringProperty(PE_LastModifiedBy, user);
}

QString PartBasedPackageProperties::revision()const
{
    return stringProperty(PE_Revision);
}

void PartBasedPackageProperties::setRevision(const QString &revision)
{
    setStringProperty(PE_Revision, revision);
}

QDateTime PartBasedPackageProperties::lastPrinted() const
{
    return dateTimeProperty(PE_LastPrinted);
}

void PartBasedPackageProperties::setLastPrinted(const QDateTime &lastPrinted)
{
    setDateTimeProperty(PE_LastPrinted, lastPrinted);
}

QDateTime PartBasedPackageProperties::created() const
{
    return dateTimeProperty(PE_Created);
}

void PartBasedPackageProperties::setCreated(const QDateTime &created)
{
    setDateTimeProperty(PE_Created, created);
}

QDateTime PartBasedPackageProperties::modified() const
{
    return dateTimeProperty(PE_Modified);
}

void PartBasedPackageProperties::setModified(const QDateTime &modified)
{
    setDateTimeProperty(PE_Modified, modified);
}


//Document summary information
QString PartBasedPackageProperties::category() const
{
    return stringProperty(PE_Category);
}

void PartBasedPackageProperties::setCategory(const QString &category)
{
    setStringProperty(PE_Category, category);
}

QString PartBasedPackageProperties::identifier() const
{
    return stringProperty(PE_Identifier);
}

void PartBasedPackageProperties::setIdentifier(const QString &identifier)
{
    setStringProperty(PE_Identifier, identifier);
}

QString PartBasedPackageProperties::contentType() const
{
    return stringProperty(PE_ContentType);
}

void PartBasedPackageProperties::setContentType(const QString &contentType)
{
    setStringProperty(PE_ContentType, contentType);
}

QString PartBasedPackageProperties::language() const
{
    return stringProperty(PE_Language);
}

void PartBasedPackageProperties::setLanguage(const QString &language)
{
    setStringProperty(PE_Language, language);
}

QString PartBasedPackageProperties::version() const
{
    return stringProperty(PE_Version);
}

void PartBasedPackageProperties::setVersion(const QString &version)
{
    setStringProperty(PE_Version, version);
}

QString PartBasedPackageProperties::contentStatus() const
{
    return stringProperty(PE_ContentStatus);
}

void PartBasedPackageProperties::setContentStatus(const QString &contentStatus)
{
    setStringProperty(PE_ContentStatus, contentStatus);
}


} // namespace Opc
} // namespace QtOfficeOpenXml
