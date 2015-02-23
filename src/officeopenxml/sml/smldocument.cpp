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
#include <private/smldocument_p.h>
#include <private/smlworksheet_p.h>
#include <private/smlworkbook_p.h>
#include <private/smlworksheetxmlpart_p.h>
#include <private/smlworkbookxmlpart_p.h>
#include <QtOfficeOpenXml/smlcell.h>
#include <QtOfficeOpenXml/smlcellreference.h>
#include <QtOfficeOpenXml/opcpackage.h>
#include <QtOfficeOpenXml/opcpackagepart.h>

#include <QtCore/qscopedpointer.h>

namespace QtOfficeOpenXml {
namespace Sml {

DocumentPrivate::DocumentPrivate(Document *q) :
    Ooxml::AbstractDocumentPrivate(q)
{
    if (packageName.isEmpty())
        packageName = QStringLiteral("Book1.xlsx");
    lastSheetId = 0;
}

/*
 * Load all the contents of the package into the memory.
 * So, this method can not be used to load very large .xlsx file.
 */
bool DocumentPrivate::doLoadPackage(Opc::Package *package)
{
    Q_ASSERT(package);
    Opc::PackageRelationship *mainRelationship = getRootRelationship(package, Ooxml::RS_OfficeDocument_OfficeDocument);
    if (!mainRelationship)
        return false;
    Opc::PackagePart *mainPart = package->part(mainRelationship->target());
    if (!mainPart)
        return false;
    if (detectedDocumentType(mainPart) != SpreadsheetDocumentType)
        return false;

    //OK, load workbookPart now.
    WorkbookXmlPart wbPart(&wb, mainPart->partName(), package);
    wbPart.loadFromPackage(ooxmlSchame);

    //Load sheets
    for (int i=0; i < wbPart.sheets.size(); ++i) {
        QHash<QString, QString> sheetInfo = wbPart.sheets[i];
        const QString name = sheetInfo[QStringLiteral("name")];
        int sheetId = sheetInfo[QStringLiteral("sheetId")].toInt();
        SheetState ss = SS_Visible;
        if (sheetInfo.contains(QStringLiteral("state"))) {
            const QString state = sheetInfo[QStringLiteral("state")];
            if (state == QLatin1String("hidden"))
                ss = SS_Hidden;
            else if (state == QLatin1String("veryHidden"))
                ss = SS_VeryHidden;
        }

        if (sheetId > lastSheetId)
            lastSheetId = sheetId;

        Opc::PackageRelationship *relationship = wbPart.packagePart()->relationship(sheetInfo[QStringLiteral("r:id")]);

        if (relationship->relationshipType() == Ooxml::Schames::relationshipUri(Ooxml::RS_OfficeDocument_Worksheet, ooxmlSchame)) {
            Worksheet *sheet  = new Worksheet(name, sheetId, ss);
            WorksheetXmlPart xmlPart(sheet->d_func(), relationship->target(), package);
            xmlPart.loadFromPackage(ooxmlSchame);
            sheets.append(QSharedPointer<AbstractSheet>(sheet));
        } else if (relationship->relationshipType() == Ooxml::Schames::relationshipUri(Ooxml::RS_OfficeDocument_Chartsheet, ooxmlSchame)) {
        } else {
            //Unsupported relationship type.
            continue;
        }
    }


    //Load common parts of the package.
    Ooxml::AbstractDocumentPrivate::doLoadPackage(package);
    return true;
}

bool DocumentPrivate::doSavePackage(Opc::Package *package, Ooxml::SchameType schame) const
{
    Q_ASSERT(package);

    //workbook part.
    WorkbookXmlPart wbPart(&(const_cast<DocumentPrivate *>(this)->wb), QStringLiteral("/xl/workbook.xml"), package);
    package->createRelationship(wbPart.partName(), Opc::Internal, Ooxml::Schames::relationshipUri(Ooxml::RS_OfficeDocument_OfficeDocument, schame));

    //Save sheets part.
    int worksheetIndex = 1;
    int chartsheetIndex = 1;
    for (int i=0; i<sheets.size(); ++i) {
        AbstractSheet *sheet = sheets[i].data();
        QString rId;
        if (sheet->sheetType() == ST_Worksheet) {
            QString partName = QStringLiteral("/xl/worksheets/sheet%1.xml").arg(worksheetIndex);
            worksheetIndex++;
            WorksheetXmlPart wsPart(static_cast<Worksheet *>(sheet)->d_func(), partName, package);
            wsPart.saveToPackage(schame);

            QString relationshipType = Ooxml::Schames::relationshipUri(Ooxml::RS_OfficeDocument_Worksheet, schame);
            rId = wbPart.packagePart()->createRelationship(partName, Opc::Internal, relationshipType)->id();
        } else if (sheet->sheetType() == ST_Chartsheet) {
            //Todo.
            chartsheetIndex++;
        }

        QHash<QString, QString> sheetInfo;
        sheetInfo[QStringLiteral("name")] = sheet->sheetName();
        sheetInfo[QStringLiteral("sheetId")] = QString::number(sheet->sheetId());
        if (sheet->sheetState() == SS_Hidden)
            sheetInfo[QStringLiteral("state")] = QStringLiteral("hidden");
        else if (sheet->sheetState() == SS_VeryHidden)
            sheetInfo[QStringLiteral("state")] = QStringLiteral("veryHidden");
        sheetInfo[QStringLiteral("r:id")] = rId;
        wbPart.sheets.append(sheetInfo);
    }

    //Save workbook part.
    wbPart.saveToPackage(schame);

    //Save common parts of the package.
    Ooxml::AbstractDocumentPrivate::doSavePackage(package, schame);
    return true;
}

QStringList DocumentPrivate::sheetNames() const
{
    QStringList names;
    for (int i=0; i<sheets.size(); ++i)
        names.append(sheets.at(i)->sheetName());
    return names;
}

Worksheet *DocumentPrivate::getWorksheetByName(const QString &sheetName) const
{
    AbstractSheet *sheet = 0;
    for (int i=0; i<sheets.size(); ++i) {
        if (sheets[i]->sheetName() == sheetName) {
            sheet = sheets[i].data();
            break;
        }
    }
    if (sheet && sheet->sheetType() == ST_Worksheet)
        return static_cast<Worksheet *>(sheet);
    return 0;
}

QSharedPointer<AbstractSheet> DocumentPrivate::createSheet(const QString &sheetName, SheetType type)
{
    lastSheetId++;
    if (type == ST_Worksheet) {
        QSharedPointer<AbstractSheet> sheet(new Worksheet(sheetName, lastSheetId, SS_Visible));
        return sheet;
    }

    return QSharedPointer<AbstractSheet>();
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

Cell *Document::cellAt(const CellReference &cell) const
{
    Q_D(const Document);
    if (cell.isNull())
        return 0;
    Worksheet *sheet = cell.sheetName().isEmpty() ? currentWorksheet() : d->getWorksheetByName(cell.sheetName());
    if (sheet)
        return sheet->cellAt(cell);
    return 0;
}

bool Document::defineName(const QString &name, const QString &formula, const QString &comment, const QString &scope)
{
    return false;
}

int Document::sheetCount() const
{
    Q_D(const Document);
    return d->sheets.count();
}

AbstractSheet *Document::addSheet(const QString &sheetName, SheetType type)
{
    return insertSheet(sheetCount(), sheetName, type);
}

AbstractSheet *Document::insertSheet(int index, const QString &sheetName, SheetType type)
{
    Q_D(Document);
    if (d->sheetNames().contains(sheetName))
        return 0;
    QSharedPointer<AbstractSheet> sheet = d->createSheet(sheetName, type);
    d->sheets.insert(index, sheet);

    //Set as active sheet.
    d->wb.setActiveTab(index);
    return sheet.data();
}

bool Document::setCurrentSheet(int index)
{
    Q_D(Document);

    if (index <0 || index >= d->sheets.size())
        return false;
    d->wb.setActiveTab(index);
    return true;
}

AbstractSheet *Document::sheet(int index) const
{
    Q_D(const Document);
    if (index <0 || index >= d->sheets.size())
        return 0;
    return d->sheets.at(index).data();
}

AbstractSheet *Document::currentSheet() const
{
    Q_D(const Document);
    return sheet(d->wb.activeTab());
}

Worksheet *Document::currentWorksheet() const
{
    AbstractSheet *s = currentSheet();
    if (s && s->sheetType() == ST_Worksheet)
        return static_cast<Worksheet *>(s);
    return 0;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
