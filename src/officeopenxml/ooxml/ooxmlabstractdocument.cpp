/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
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
#include <private/ooxmlabstractdocument_p.h>
#include <private/ooxmldocpropsapppart_p.h>
#include <QtOfficeOpenXml/opcpackage.h>
#include <QtOfficeOpenXml/opcpackagepart.h>
#include <QtOfficeOpenXml/opcpackageproperties.h>

#include <QtCore/qscopedpointer.h>
#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>
#include <QtCore/qdatetime.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

AbstractDocumentPrivate::AbstractDocumentPrivate(AbstractDocument *q) :
    ooxmlSchame(UnknownSchame),
    q_ptr(q)
{
}

AbstractDocumentPrivate::~AbstractDocumentPrivate()
{

}

/*
 * Load ooxml package parts shared by spreadsheet/wordprocessing/presentation.
 *
 * Note,
 * - docProps/core.xml is part of Opc::Package, so it's a common part.
 * - docProps/app.xml is common part for .xlsx/.docx/.pptx too.
 */
bool AbstractDocumentPrivate::doLoadPackage(Opc::Package *package)
{
    //-- First step. --
    //Extract core properties from Opc::Package, or we can load the xml part directly here.
    Opc::PackageProperties *coreProperties = package->packageProperties();

    QMap<int, QString (Opc::PackageProperties::*)() const> dpFunctionMap;
    dpFunctionMap[AbstractDocument::DP_Creator] = &Opc::PackageProperties::creator;
    dpFunctionMap[AbstractDocument::DP_Identifier] = &Opc::PackageProperties::identifier;
    dpFunctionMap[AbstractDocument::DP_Title] = &Opc::PackageProperties::title;
    dpFunctionMap[AbstractDocument::DP_Subject] = &Opc::PackageProperties::subject;
    dpFunctionMap[AbstractDocument::DP_Description] = &Opc::PackageProperties::description;
    dpFunctionMap[AbstractDocument::DP_Language] = &Opc::PackageProperties::language;
    dpFunctionMap[AbstractDocument::DP_ContentType] = &Opc::PackageProperties::contentType;
    dpFunctionMap[AbstractDocument::DP_Keywords] = &Opc::PackageProperties::keywords;
    dpFunctionMap[AbstractDocument::DP_Category] = &Opc::PackageProperties::category;
    dpFunctionMap[AbstractDocument::DP_LastModifiedBy] = &Opc::PackageProperties::lastModifiedBy;
    dpFunctionMap[AbstractDocument::DP_ContentStatus] = &Opc::PackageProperties::contentStatus;
    dpFunctionMap[AbstractDocument::DP_Revision] = &Opc::PackageProperties::revision;
    dpFunctionMap[AbstractDocument::DP_Version] = &Opc::PackageProperties::version;
    QMapIterator<int, QString (Opc::PackageProperties::*)() const> it(dpFunctionMap);
    while(it.hasNext()) {
        it.next();
        QString val = (coreProperties->*(it.value()))();
        if (!val.isNull())
            documentPropertyHash.insert(it.key(), val);
    }
    //Three other members which returns QDateTime instead of QString.
    if (!coreProperties->created().isNull())
        documentPropertyHash.insert(AbstractDocument::DP_Created, coreProperties->created());
    if (!coreProperties->modified().isNull())
        documentPropertyHash.insert(AbstractDocument::DP_Modified, coreProperties->modified());
    if (!coreProperties->lastPrinted().isNull())
        documentPropertyHash.insert(AbstractDocument::DP_LastPrinted, coreProperties->lastPrinted());

    //-- Second step. --
    //Load app properties part if exists.
    Opc::PackagePart *docPropsAppPackagePart = getPackageRootPart(package, RS_OfficeDocument_Extended);
    if (docPropsAppPackagePart) {
        DocPropsAppPart docPropsAppPart(this);
        docPropsAppPart.loadFromPackagePart(docPropsAppPackagePart);
    }

    return true;
}

/*
 * Save ooxml package parts shared by spreadsheet/wordprocessing/presentation.
 */
bool AbstractDocumentPrivate::doSavePackage(Opc::Package *package, SchameType schameType) const
{
    //-- First step. --
    //Write core properties to Opc::Package, or we can createPart directly here.
    Opc::PackageProperties *coreProperties = package->packageProperties();
    QMap<int, void (Opc::PackageProperties::*)(const QString &)> dpFunctionMap;
    dpFunctionMap[AbstractDocument::DP_Creator] = &Opc::PackageProperties::setCreator;
    dpFunctionMap[AbstractDocument::DP_Identifier] = &Opc::PackageProperties::setIdentifier;
    dpFunctionMap[AbstractDocument::DP_Title] = &Opc::PackageProperties::setTitle;
    dpFunctionMap[AbstractDocument::DP_Subject] = &Opc::PackageProperties::setSubject;
    dpFunctionMap[AbstractDocument::DP_Description] = &Opc::PackageProperties::setDescription;
    dpFunctionMap[AbstractDocument::DP_Language] = &Opc::PackageProperties::setLanguage;
    dpFunctionMap[AbstractDocument::DP_ContentType] = &Opc::PackageProperties::setContentType;
    dpFunctionMap[AbstractDocument::DP_Keywords] = &Opc::PackageProperties::setKeywords;
    dpFunctionMap[AbstractDocument::DP_Category] = &Opc::PackageProperties::setCategory;
    dpFunctionMap[AbstractDocument::DP_LastModifiedBy] = &Opc::PackageProperties::setLastModifiedBy;
    dpFunctionMap[AbstractDocument::DP_ContentStatus] = &Opc::PackageProperties::setContentStatus;
    dpFunctionMap[AbstractDocument::DP_Revision] = &Opc::PackageProperties::setRevision;
    dpFunctionMap[AbstractDocument::DP_Version] = &Opc::PackageProperties::setVersion;

    QMapIterator<int, void (Opc::PackageProperties::*)(const QString &)> it(dpFunctionMap);
    while(it.hasNext()) {
        it.next();
        if (documentPropertyHash.contains(it.key()))
            (coreProperties->*(it.value()))(documentPropertyHash[it.key()].toString());
    }
    //Three other members which returns QDateTime instead of QString.
    if (documentPropertyHash.contains(AbstractDocument::DP_Created))
        coreProperties->setCreated(documentPropertyHash[AbstractDocument::DP_Created].toDateTime());
    if (documentPropertyHash.contains(AbstractDocument::DP_Modified))
        coreProperties->setModified(documentPropertyHash[AbstractDocument::DP_Modified].toDateTime());
    if (documentPropertyHash.contains(AbstractDocument::DP_LastPrinted))
        coreProperties->setLastPrinted(documentPropertyHash[AbstractDocument::DP_LastPrinted].toDateTime());

    //-- Second step. --
    //Create app properties part.
    Opc::PackagePart *docPropsAppPackagePart = package->createPart(QStringLiteral("/docProps/app.xml"),
                                                                   QStringLiteral("application/vnd.openxmlformats-officedocument.extended-properties+xml"));
    if (docPropsAppPackagePart) {
        DocPropsAppPart docPropsAppPart(const_cast<AbstractDocumentPrivate*>(this));
        docPropsAppPart.saveToPackagePart(docPropsAppPackagePart, schameType);
    }

    return true;
}

/* Get package root part based on root relationship. If more that one parts exist, the return value is undefined.
 */
Opc::PackagePart *AbstractDocumentPrivate::getPackageRootPart(Opc::Package *package, RelationshipId relationshipId)
{
    Opc::PackageRelationship *relationship = 0;
    if (ooxmlSchame == UnknownSchame) {
        //Try find the schame used by this package.
        if ((relationship = package->getRelationshipByType(Schames::relationshipUri(relationshipId, TransitionalSchame)))) {
            ooxmlSchame = TransitionalSchame;
        } else if ((relationship = package->getRelationshipByType(Schames::relationshipUri(relationshipId, StrictSchame)))) {
            ooxmlSchame = StrictSchame;
        }
    } else {
        relationship = package->getRelationshipByType(Schames::relationshipUri(relationshipId, ooxmlSchame));
    }

    if (!relationship)
        return 0;

    return package->part(relationship->target());
}

SchameType AbstractDocumentPrivate::getFixedSaveAsSchame(SchameType schame) const
{
    if (schame != UnknownSchame)
        return schame;
    if (ooxmlSchame != UnknownSchame)
        return ooxmlSchame;
    return TransitionalSchame;
}

/*
 * Find office document type based on the content type of main part.
 */
AbstractDocumentPrivate::DocumentType AbstractDocumentPrivate::detectedDocumentType(Opc::PackagePart *mainPart)
{
    QHash<QString, DocumentType> typesHash;

    // known content types for wordprocessing document
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"), WordprocessingDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml"), WordprocessingDocumentType);

    // known content types for spreadsheet document
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"), SpreadsheetDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml"), SpreadsheetDocumentType);

    // known content types for presentation document
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml"), PresentationDocumentType);

#if 0
    // Non ISO standard content types, which is supported by Microsoft Office 2007.
    // As we won't support these content types, so simply treat them as Invalid.
    typesHash.insert(QStringLiteral("application/vnd.ms-word.document.macroEnabled.main+xml"), WordprocessingDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-word.template.macroEnabledTemplate.main+xml"), WordprocessingDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-excel.sheet.macroEnabled.main+xml"), SpreadsheetDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-excel.template.macroEnabled.main+xml"), SpreadsheetDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-excel.addin.macroEnabled.main+xml"), SpreadsheetDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.template.macroEnabled.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.addin.macroEnabled.main+xml"), PresentationDocumentType);
#endif

    if (typesHash.contains(mainPart->contentType()))
        return typesHash[mainPart->contentType()];

    return InvalidDocumentType;
}

/*! \class QtOfficeOpenXml::Ooxml::Document
 *
 *  \brief For office document loading and saving.
 *
 *  This is the base class for QtOfficeOpenXml::Sml::Document,
 *  QtOfficeOpenXml::Wml::Document and QtOfficeOpenXml::Pml::Document,
 */

/*!
 * \internal
 * Designed to be called by subclasses of the class.
 */
AbstractDocument::AbstractDocument(AbstractDocumentPrivate *d, QObject *parent) :
    QObject(parent), d_ptr(d)
{
}

/*!
 * Destroy this document.
 */
AbstractDocument::~AbstractDocument()
{
    delete d_ptr;
}

/*!
 * Return the document property of \a name.
 */
QVariant AbstractDocument::documentProperty(AbstractDocument::DocumentProperty name) const
{
    Q_D(const AbstractDocument);
    if (d->documentPropertyHash.contains(name))
        return d->documentPropertyHash[name];
    return QVariant();
}

/*!
 * Set the document property of \a name to value \a property.
 */
void AbstractDocument::setDocumentProperty(AbstractDocument::DocumentProperty name, const QVariant &property)
{
    Q_D(AbstractDocument);
    if (property.isNull() && d->documentPropertyHash.contains(name)) {
        d->documentPropertyHash.remove(name);
        return;
    }
    d->documentPropertyHash[name] = property;
}

/*!
 * Save the document using the given ooxml schame \a schameType.
 * Returns true on success.
 */
bool AbstractDocument::save(SchameType schameType) const
{
    Q_D(const AbstractDocument);
    if (d->packageName.isEmpty())
        return false;
    return saveAs(d->packageName, schameType);
}

/*!
 * Save the document to \a fileName using the given ooxml schame \a schameType.
 * Returns true on success.
 */
bool AbstractDocument::saveAs(const QString &fileName, SchameType schameType) const
{
    Q_D(const AbstractDocument);
    QScopedPointer<Opc::Package> package(Opc::Package::open(fileName, QIODevice::WriteOnly));
    bool ret = false;
    if (package)
        ret = d->doSavePackage(package.data(), d->getFixedSaveAsSchame(schameType));
    return ret;
}

/*!
 * \overload
 * Save the document to \a device using the given ooxml schame \a schameType.
 * Returns true on success.
 *
 * \warning The \a device will be closed when this function returned.
 * So be careful when used with QTcpSocket or similiar devices.
 */
bool AbstractDocument::saveAs(QIODevice *device, SchameType schameType) const
{
    Q_D(const AbstractDocument);
    QScopedPointer<Opc::Package> package(Opc::Package::open(device, QIODevice::WriteOnly));
    bool ret = false;
    if (package)
        ret = d->doSavePackage(package.data(), d->getFixedSaveAsSchame(schameType));
    return ret;
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
