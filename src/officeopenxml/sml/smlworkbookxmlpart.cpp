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
#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtOfficeOpenXml/mcexmlstreamreader.h>

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

bool WorkbookXmlPart::doLoadFromXml(QIODevice *part)
{
    Mce::XmlStreamReader reader(part);
    //Should we add all the understood names used by this part here?
    reader.setMceParseFlag(Mce::PF_AllowNonUnderstoodNonIngorableNamespaces);
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    while (reader.readNextStartElement()) {
        if (reader.name() == QLatin1String("sheet")) {
            QXmlStreamAttributes attributes = reader.attributes();
            const QString name = attributes.value(QLatin1String("name")).toString();
            int sheetId = attributes.value(QLatin1String("sheetId")).toString().toInt();
            const QString rId = attributes.value(QLatin1String("r:id")).toString();
            const QStringRef &stateString = attributes.value(QLatin1String("state"));
            SheetState state = SS_Visible;
            if (stateString == QLatin1String("hidden"))
                state = SS_Hidden;
            else if (stateString == QLatin1String("veryHidden"))
                state = SS_VeryHidden;
            sheets.append(Sheet(name, sheetId, state, rId));
        } else if (reader.name() == QLatin1String("workbookPr")) {
            foreach (QXmlStreamAttribute attr, reader.attributes())
                wb->workbookPr.insert(attr.name().toString(), attr.value().toString());
        } else if (reader.name() == QLatin1String("workbookView")) {
            QHash<QString, QString> attrsHash;
            foreach (QXmlStreamAttribute attr, reader.attributes())
                attrsHash.insert(attr.name().toString(), attr.value().toString());
            wb->bookViews.append(attrsHash);
        } else if (reader.name() == QLatin1String("externalReference")) {
            externalReferences.append(reader.attributes().value(QLatin1String("r:id")).toString());
        } else if (reader.name() == QLatin1String("definedName")) {
            QHash<QString, QString> attrsHash;
            foreach (QXmlStreamAttribute attr, reader.attributes())
                attrsHash.insert(attr.name().toString(), attr.value().toString());
            wb->definedNames.append(DefinedName(attrsHash, reader.readElementText()));
        }
    }
    return true;
}

bool WorkbookXmlPart::doSaveToXml(QIODevice *part, Ooxml::SchameType schameType) const
{
    QXmlStreamWriter writer(part);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("workbook"));
    writer.writeDefaultNamespace(Ooxml::Schames::namespaceUri(Ooxml::NS_Spreadsheetml_Main, schameType));
    writer.writeNamespace(Ooxml::Schames::namespaceUri(Ooxml::NS_OfficeDocument_Relationships, schameType), QStringLiteral("r"));

    if (!wb->fileVersion.isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("fileVersion"));
        QHashIterator<QString, QString> it(wb->fileVersion);
        while(it.hasNext()) {
            it.next();
            writer.writeAttribute(it.key(), it.value());
        }
    }

    if (!wb->workbookPr.isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("workbookPr"));
        QHashIterator<QString, QString> it(wb->workbookPr);
        while(it.hasNext()) {
            it.next();
            writer.writeAttribute(it.key(), it.value());
        }
    }

    if (!wb->bookViews.isEmpty()) {
        writer.writeStartElement(QStringLiteral("bookViews"));
        for(int i=0; i<wb->bookViews.size(); ++i) {
            writer.writeEmptyElement(QStringLiteral("workbookView"));
            QHashIterator<QString, QString> it(wb->bookViews[i]);
            while(it.hasNext()) {
                it.next();
                writer.writeAttribute(it.key(), it.value());
            }
        }
#if 0
        //Store the firstSheet when it isn't the default
        //For example, when "the first sheet 0 is hidden", the first sheet will be 1
        if (d->firstsheet > 0)
            writer.writeAttribute(QStringLiteral("firstSheet"), QString::number(d->firstsheet + 1));
        //Store the activeTab when it isn't the first sheet
        if (d->activesheetIndex > 0)
            writer.writeAttribute(QStringLiteral("activeTab"), QString::number(d->activesheetIndex));
#endif
        writer.writeEndElement();//bookViews
    }

    //At least sheet exists.
    writer.writeStartElement(QStringLiteral("sheets"));
    for (int i=0; i<sheets.size(); ++i) {
        writer.writeEmptyElement(QStringLiteral("sheet"));
        writer.writeAttribute(QStringLiteral("name"), sheets[i].name);
        writer.writeAttribute(QStringLiteral("sheetId"), QString::number(sheets[i].sheetId));
        if (sheets[i].state == SS_Hidden)
            writer.writeAttribute(QStringLiteral("state"), QStringLiteral("hidden"));
        else if (sheets[i].state == SS_VeryHidden)
            writer.writeAttribute(QStringLiteral("state"), QStringLiteral("veryHidden"));
        writer.writeAttribute(QStringLiteral("r:id"), sheets[i].rId);
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
        foreach (DefinedName data, wb->definedNames) {
            writer.writeStartElement(QStringLiteral("definedName"));
            QHashIterator<QString, QString> it(data.attrs);
            while(it.hasNext()) {
                it.next();
                writer.writeAttribute(it.key(), it.value());
            }

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
