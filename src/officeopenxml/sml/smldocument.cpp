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
#include "smldocument.h"
#include "smldocument_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"

#include <QtCore/qscopedpointer.h>

namespace QtOfficeOpenXml {
namespace Sml {

DocumentPrivate::DocumentPrivate(Document *q) :
    Ooxml::AbstractDocumentPrivate(q)
{
    if (packageName.isEmpty())
        packageName = QStringLiteral("Book1.xlsx");
}

/*
 * Load all the contents of the package into the memory.
 * So, this method can not be used to load very large .xlsx file.
 */
bool DocumentPrivate::doLoadPackage(Opc::Package *package)
{
    Q_ASSERT(package);
    Opc::PackagePart *mainPart = getPackageRootPart(package, Ooxml::RS_OfficeDocument_OfficeDocument);
    if (!mainPart)
        return false;
    if (detectedDocumentType(mainPart) != SpreadsheetDocumentType)
        return false;
    //Todo, load workbookPart now.

    //Load common parts of the package.
    Ooxml::AbstractDocumentPrivate::doLoadPackage(package);
    return true;
}

bool DocumentPrivate::doSavePackage(Opc::Package *package, Ooxml::SchameType schame) const
{
    Q_ASSERT(package);
    //Todo, save workbook related parts.

    //Save common parts of the package.
    Ooxml::AbstractDocumentPrivate::doSavePackage(package, schame);
    return true;
}


/*! \class QtOfficeOpenXml::Sml::Document
 *
 *  \brief For .xlsx loading and saving.
 */

Document::Document(QObject *parent) :
    Ooxml::AbstractDocument(new DocumentPrivate(this), parent)
{
}

Document::Document(const QString &fileName, QObject *parent) :
    Ooxml::AbstractDocument(new DocumentPrivate(this), parent)
{
    //Save this fileName, which will be used by save() member.
    d_func()->packageName = fileName;
    QScopedPointer<Opc::Package> package(Opc::Package::open(fileName, QIODevice::ReadOnly));
    if (package)
        d_func()->doLoadPackage(package.data());
}

Document::Document(QIODevice *device, QObject *parent) :
    Ooxml::AbstractDocument(new DocumentPrivate(this), parent)
{
    QScopedPointer<Opc::Package> package(Opc::Package::open(device, QIODevice::ReadOnly));
    if (package)
        d_func()->doLoadPackage(package.data());
}

Document::~Document()
{
}

} // namespace Sml
} // namespace QtOfficeOpenXml
