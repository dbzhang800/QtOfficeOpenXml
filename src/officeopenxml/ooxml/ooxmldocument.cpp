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

namespace QtOfficeOpenXml {
namespace Ooxml {

DocumentPrivate::DocumentPrivate(Document *q) :
    ooxmlSchame(TransitionalSchame),
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

    if (strict)
        ooxmlSchame = StrictSchame;//Mark schame used by this part.
    return mainPart;
}

DocumentPrivate::DocumentType DocumentPrivate::detectedDocumentType(Opc::PackagePart *mainPart)
{
    QHash<QString, DocumentType> typesHash;

    // known content types for wordprocessing document
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"), WordprocessingDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml"), WordprocessingDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-word.document.macroEnabled.main+xml"), WordprocessingDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-word.template.macroEnabledTemplate.main+xml"), WordprocessingDocumentType);

    // known content types for spreadsheet document
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"), SpreadsheetDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml"), SpreadsheetDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-excel.sheet.macroEnabled.main+xml"), SpreadsheetDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-excel.template.macroEnabled.main+xml"), SpreadsheetDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-excel.addin.macroEnabled.main+xml"), SpreadsheetDocumentType);

    // known content types for presentation document
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml"), PresentationDocumentType);
    typesHash.insert(QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml"), PresentationDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml"), PresentationDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.template.macroEnabled.main+xml"), PresentationDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml"), PresentationDocumentType);
    //typesHash.insert(QStringLiteral("application/vnd.ms-powerpoint.addin.macroEnabled.main+xml"), PresentationDocumentType);

    if (typesHash.contains(mainPart->contentType()))
        return typesHash[mainPart->contentType()];

    return InvalidDocumentType;
}


/*! \class QtOfficeOpenXml::Ooxml::Document
 *
 *  \brief For office document loading and saving.
 */


Document::Document(DocumentPrivate *d, QObject *parent) :
    QObject(parent), d_ptr(d)
{
}

Document::~Document()
{
    delete d_ptr;
}

bool Document::save(SchameType schameType) const
{
    Q_D(const Document);
    if (d->packageName.isEmpty())
        return false;
    return saveAs(d->packageName, schameType);
}

bool Document::saveAs(const QString &fileName, SchameType schameType) const
{
    Q_D(const Document);
    QScopedPointer<Opc::Package> package(Opc::Package::open(fileName, QIODevice::WriteOnly));
    bool ret = false;
    if (package)
        ret = d->doSavePackage(package.data(), schameType);
    return ret;
}

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
