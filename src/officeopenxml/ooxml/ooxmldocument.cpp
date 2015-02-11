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
#include "ooxmldocument.h"
#include "ooxmldocument_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"

#include <QtCore/qscopedpointer.h>
#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

DocumentPrivate::DocumentPrivate(Document *q) :
    ooxmlSchame(UnknownSchame),
    q_ptr(q)
{
}

DocumentPrivate::~DocumentPrivate()
{

}

/*
 * Load ooxml package parts shared by spreadsheet/wordprocessing/presentation.
 */
bool DocumentPrivate::doLoadPackage(Opc::Package *package)
{
    return false;
}

/*
 * Save ooxml package parts shared by spreadsheet/wordprocessing/presentation.
 */
bool DocumentPrivate::doSavePackage(Opc::Package *package, SchameType schameType) const
{
    return false;
}

Opc::PackagePart *DocumentPrivate::findMainPartFromPackage(Opc::Package *package)
{
    Q_ASSERT(package);

    //Find office main part of the package.
    bool strict = false;
    Opc::PackageRelationship *mainPartRelationship = package->getRelationshipByType(Schames::relationshipUri(RS_OfficeDocument_OfficeDocument, TransitionalSchame));
    if (!mainPartRelationship) {
        //Try strict schame.
        mainPartRelationship = package->getRelationshipByType(Schames::relationshipUri(RS_OfficeDocument_OfficeDocument, StrictSchame));
        if (mainPartRelationship)
            strict = true;
    }
    if (!mainPartRelationship)
        return 0;
    Opc::PackagePart *mainPart = package->part(mainPartRelationship->target());
    if (!mainPart)
        return 0;

    //Mark schame used by this part if main part is found.
    ooxmlSchame = strict ? StrictSchame : TransitionalSchame;
    return mainPart;
}

/*
 * Find office document type based on the content type of main part.
 */
DocumentPrivate::DocumentType DocumentPrivate::detectedDocumentType(Opc::PackagePart *mainPart)
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
Document::Document(DocumentPrivate *d, QObject *parent) :
    QObject(parent), d_ptr(d)
{
}

/*!
 * Destroy this document.
 */
Document::~Document()
{
    delete d_ptr;
}

/*!
 * Return the document property of \a name.
 */
QVariant Document::documentProperty(Document::DocumentProperty name) const
{
    Q_D(const Document);
    if (d->documentPropertyHash.contains(name))
        return d->documentPropertyHash[name];
    return QVariant();
}

/*!
 * Set the document property of \a name to value \a property.
 */
void Document::setDocumentProperty(Document::DocumentProperty name, const QVariant &property)
{
    Q_D(Document);
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
bool Document::save(SchameType schameType) const
{
    Q_D(const Document);
    if (d->packageName.isEmpty())
        return false;
    return saveAs(d->packageName, schameType);
}

/*!
 * Save the document to \a fileName using the given ooxml schame \a schameType.
 * Returns true on success.
 */
bool Document::saveAs(const QString &fileName, SchameType schameType) const
{
    Q_D(const Document);
    QScopedPointer<Opc::Package> package(Opc::Package::open(fileName, QIODevice::WriteOnly));
    bool ret = false;
    if (package)
        ret = d->doSavePackage(package.data(), schameType);
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
bool Document::saveAs(QIODevice *device, SchameType schameType) const
{
    Q_D(const Document);
    QScopedPointer<Opc::Package> package(Opc::Package::open(device, QIODevice::WriteOnly));
    bool ret = false;
    if (package)
        ret = d->doSavePackage(package.data(), schameType);
    return ret;
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
