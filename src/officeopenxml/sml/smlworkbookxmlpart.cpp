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
#include <private/smlworkbookxmlpart_p.h>
#include <private/smlworkbook_p.h>
#include <private/ooxmlxmlhelper_p.h>
#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtOfficeOpenXml/mcexmlstreamreader.h>

#include <QtCore/qdebug.h>

namespace QtOfficeOpenXml {
namespace Sml {

WorkbookXmlPart::WorkbookXmlPart(Workbook *wb, const QString &partName, QtOfficeOpenXml::Opc::Package *package) :
    AbstractFixedTypeXmlPart(partName, package), wb(wb)
{
}

QString WorkbookXmlPart::contentType() const
{
    //application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml
    return QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
}

bool WorkbookXmlPart::doLoadFromXml(QIODevice *device, Ooxml::SchameType schameType)
{
    Mce::XmlStreamReader reader(device);
    //Should we add all the understood names used by this part here?
    reader.setMceParseFlag(Mce::PF_AllowNonUnderstoodNonIngorableNamespaces);
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    //It's not safe to used "r:Id", so we must use attribute name and attribute namespaceUri.
    QString r = Ooxml::Schames::namespaceUri(Ooxml::NS_OfficeDocument_Relationships, schameType);

    while (reader.readNextStartElement()) {
        if (reader.name() == QLatin1String("fileVersion")) {
            wb->fileVersion_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
        } else if (reader.name() == QLatin1String("fileSharing")) {
            wb->fileSharing_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
        } else if (reader.name() == QLatin1String("workbookPr")) {
            wb->workbookPr_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
        } else if (reader.name() == QLatin1String("workbookProtection")) {
            wb->workbookProtection_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
        } else if (reader.name() == QLatin1String("workbookView")) {
            wb->bookViews_raw.append(Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes()));
        } else if (reader.name() == QLatin1String("sheet")) {
            //Elements contains "r:Id" is not saved in Workbook (wb).
            sheets.append(Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes()));
        } else if (reader.name() == QLatin1String("externalReference")) {
            //Elements contains "r:Id" is not saved in Workbook (wb).
            externalReferences.append(reader.attributes().value(r, QLatin1String("id")).toString());
        } else if (reader.name() == QLatin1String("definedName")) {
            wb->definedNames.append(DefinedName(Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes()), reader.readElementText()));
        }
    }
    return true;
}

bool WorkbookXmlPart::doSaveToXml(QIODevice *device, Ooxml::SchameType schameType) const
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("workbook"));
    writer.writeDefaultNamespace(Ooxml::Schames::namespaceUri(Ooxml::NS_Spreadsheetml_Main, schameType));
    writer.writeNamespace(Ooxml::Schames::namespaceUri(Ooxml::NS_OfficeDocument_Relationships, schameType), QStringLiteral("r"));

    if (!wb->fileVersion_raw.isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("fileVersion"));
        QStringList attrNames = QStringList() << QStringLiteral("appName")
                                              << QStringLiteral("lastEdited")
                                              << QStringLiteral("lowestEdited")
                                              << QStringLiteral("rupBuild")
                                              << QStringLiteral("codeName");
        writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(wb->fileVersion_raw, attrNames));
    }

    if (!wb->fileSharing_raw.isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("fileSharing"));
        QStringList attrNames = QStringList() << QStringLiteral("readOnlyRecommended")
                                              << QStringLiteral("userName")
                                              << QStringLiteral("reservationPassword");
        writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(wb->fileSharing_raw, attrNames));
    }

    if (!wb->workbookPr_raw.isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("workbookPr"));
        QStringList attrNames = QStringList() << QStringLiteral("date1904") << QStringLiteral("showObjects")
                                              << QStringLiteral("showBorderUnselectedTables") << QStringLiteral("filterPrivacy")
                                              << QStringLiteral("promptedSolutions") << QStringLiteral("showInkAnnotation")
                                              << QStringLiteral("backupFile") << QStringLiteral("saveExternalLinkValues")
                                              << QStringLiteral("updateLinks") << QStringLiteral("codeName")
                                              << QStringLiteral("hidePivotFieldList") << QStringLiteral("showPivotChartFilter")
                                              << QStringLiteral("allowRefreshQuery") << QStringLiteral("publishItems")
                                              << QStringLiteral("checkCompatibility") << QStringLiteral("autoCompressPictures")
                                              << QStringLiteral("refreshAllConnections") << QStringLiteral("defaultThemeVersion");
        writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(wb->workbookPr_raw, attrNames));
    }

    if (!wb->workbookProtection_raw.isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("workbookProtection"));
        QStringList attrNames = QStringList() << QStringLiteral("workbookPassword")
                                              << QStringLiteral("revisionsPassword")
                                              << QStringLiteral("lockStructure")
                                              << QStringLiteral("lockWindows")
                                              << QStringLiteral("lockRevision");
        writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(wb->workbookProtection_raw, attrNames));
    }

    if (!wb->bookViews_raw.isEmpty()) {
        writer.writeStartElement(QStringLiteral("bookViews"));
        QStringList attrNames = QStringList() << QStringLiteral("visibility") << QStringLiteral("minimized")
                                              << QStringLiteral("showHorizontalScroll") << QStringLiteral("showVerticalScroll")
                                              << QStringLiteral("showSheetTabs")
                                              << QStringLiteral("xWindow") << QStringLiteral("yWindow")
                                              << QStringLiteral("windowWidth") << QStringLiteral("windowHeight")
                                              << QStringLiteral("tabRatio") << QStringLiteral("firstSheet")
                                              << QStringLiteral("activeTab") << QStringLiteral("autoFilterDateGrouping");

        for(int i=0; i<wb->bookViews_raw.size(); ++i) {
            writer.writeEmptyElement(QStringLiteral("workbookView"));
            writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(wb->bookViews_raw.at(i), attrNames));
        }
        writer.writeEndElement();//bookViews
    }

    //At least one sheet must exist.
    Q_ASSERT(!sheets.isEmpty());
    writer.writeStartElement(QStringLiteral("sheets"));
    QStringList sheetAttrNames = QStringList() << QStringLiteral("name")
                                          << QStringLiteral("sheetId")
                                          << QStringLiteral("state")
                                          << QStringLiteral("r:id");
    for (int i=0; i<sheets.size(); ++i) {
        writer.writeEmptyElement(QStringLiteral("sheet"));
        writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(sheets.at(i), sheetAttrNames));
    }
    writer.writeEndElement();//sheets

    if (!externalReferences.isEmpty()) {
        writer.writeStartElement(QStringLiteral("externalReferences"));
        foreach (QString rId, externalReferences) {
            writer.writeEmptyElement(QStringLiteral("externalReference"));
            writer.writeAttribute(QStringLiteral("r:id"), rId);
        }
        writer.writeEndElement();//externalReferences
    }

    if (!wb->definedNames.isEmpty()) {
        writer.writeStartElement(QStringLiteral("definedNames"));
        QStringList attrNames = QStringList() << QStringLiteral("name") << QStringLiteral("comment")
                                              << QStringLiteral("customMenu") << QStringLiteral("description")
                                              << QStringLiteral("help") << QStringLiteral("statusBar")
                                              << QStringLiteral("localSheetId") << QStringLiteral("hidden")
                                              << QStringLiteral("function") << QStringLiteral("vbProcedure")
                                              << QStringLiteral("xlm") << QStringLiteral("functionGroupId")
                                              << QStringLiteral("shortcutKey") << QStringLiteral("publishToServer")
                                              << QStringLiteral("workbookParameter");
        foreach (DefinedName data, wb->definedNames) {
            writer.writeStartElement(QStringLiteral("definedName"));
            writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(data.attrs_raw, attrNames));
            writer.writeCharacters(data.formula);
            writer.writeEndElement();//definedName
        }
        writer.writeEndElement();//definedNames
    }
#if 0
    writer.writeStartElement(QStringLiteral("calcPr"));
    writer.writeAttribute(QStringLiteral("calcId"), QStringLiteral("124519"));
    writer.writeEndElement(); //calcPr
#endif
    writer.writeEndElement();//workbook
    writer.writeEndDocument();
    return true;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
