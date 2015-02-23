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
#include <private/smlworksheetxmlpart_p.h>
#include <private/smlworksheet_p.h>
#include <private/smlcell_p.h>
#include <private/ooxmlxmlhelper_p.h>
#include <QtOfficeOpenXml/smlcellreference.h>
#include <QtOfficeOpenXml/mcexmlstreamreader.h>

namespace QtOfficeOpenXml {
namespace Sml {

WorksheetXmlPart::WorksheetXmlPart(WorksheetPrivate *worksheet, const QString &partName, QtOfficeOpenXml::Opc::Package *package) :
    AbstractFixedTypeXmlPart(partName, package), ws(worksheet)
{
}

QString WorksheetXmlPart::contentType() const
{
    return QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml");
}

bool WorksheetXmlPart::doLoadFromXml(QIODevice *device, Ooxml::SchameType schameType)
{
    Mce::XmlStreamReader reader(device);
    //Should we add all the understood names used by this part here?
    reader.setMceParseFlag(Mce::PF_AllowNonUnderstoodNonIngorableNamespaces);
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("sheetPr")) {
                loadXmlElement_sheetPr(reader);
            } else if (reader.name() == QLatin1String("dimension")) {

            } else if (reader.name() == QLatin1String("sheetView")) {
                loadXmlElement_sheetView(reader);
            } else if (reader.name() == QLatin1String("sheetFormatPr")) {
                ws->sheetFormatPr_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
            } else if (reader.name() == QLatin1String("col")) {
                int min = reader.attributes().value(QLatin1String("min")).toString().toInt();
                ws->cols_raw.insert(min, Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes()));
            } else if (reader.name() == QLatin1String("row")) {
                loadXmlElement_row(reader);
            }
        }
    }
    return true;
}

bool WorksheetXmlPart::doSaveToXml(QIODevice *device, Ooxml::SchameType schameType) const
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("worksheet"));
    writer.writeDefaultNamespace(Ooxml::Schames::namespaceUri(Ooxml::NS_Spreadsheetml_Main, schameType));
    writer.writeNamespace(Ooxml::Schames::namespaceUri(Ooxml::NS_OfficeDocument_Relationships, schameType), QStringLiteral("r"));

    if (!ws->sheetPr.isEmpty()) {
        writer.writeStartElement(QStringLiteral("sheetPr"));
        if (!ws->sheetPr.attrs_raw.isEmpty()) {
            QStringList names = QStringList() << QStringLiteral("syncHorizontal")
                                             << QStringLiteral("syncVertical")
                                             << QStringLiteral("syncRef")
                                             << QStringLiteral("transitionEvaluation")
                                             << QStringLiteral("transitionEntry")
                                             << QStringLiteral("published")
                                             << QStringLiteral("codeName")
                                             << QStringLiteral("filterMode")
                                             << QStringLiteral("enableFormatConditionsCalculation");
            writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(ws->sheetPr.attrs_raw, names));
        }
        if (!ws->sheetPr.tabColor_raw.isEmpty()) {
            QStringList names = QStringList() << QStringLiteral("auto")
                                             << QStringLiteral("indexed")
                                             << QStringLiteral("rgb")
                                             << QStringLiteral("theme")
                                             << QStringLiteral("tint");
            writer.writeEmptyElement(QStringLiteral("tabColor"));
            writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(ws->sheetPr.tabColor_raw, names));
        }
        if (!ws->sheetPr.outlinePr_raw.isEmpty()) {
            QStringList names = QStringList() << QStringLiteral("applyStyles")
                                             << QStringLiteral("summaryBelow")
                                             << QStringLiteral("summaryRight")
                                             << QStringLiteral("showOutlineSymbols");
            writer.writeEmptyElement(QStringLiteral("outlinePr"));
            writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(ws->sheetPr.outlinePr_raw, names));
        }
        if (!ws->sheetPr.pageSetUpPr_raw.isEmpty()) {
            QStringList names = QStringList() << QStringLiteral("autoPageBreaks")
                                             << QStringLiteral("fitToPage");
            writer.writeEmptyElement(QStringLiteral("pageSetUpPr"));
            writer.writeAttributes(Ooxml::XmlHelper::xmlAttributesFromHash(ws->sheetPr.pageSetUpPr_raw, names));
        }
        writer.writeEndElement(); //sheetPr
    }

    //Write celldata
    writer.writeStartElement(QStringLiteral("sheetData"));
    QMapIterator<int, QMap<int, QSharedPointer<Cell> > > rowIt(ws->cellTable);
    while(rowIt.hasNext()) {
        rowIt.next();
        writer.writeStartElement(QStringLiteral("row"));
        writer.writeAttribute(QStringLiteral("r"), QString::number(rowIt.key()));
        QMapIterator<int, QSharedPointer<Cell> > columnIt(rowIt.value());
        while(columnIt.hasNext()) {
            columnIt.next();
            writer.writeStartElement(QStringLiteral("c"));
            writer.writeAttribute(QStringLiteral("r"), CellReference(rowIt.key(), columnIt.key()).toString());
            Cell *cell = columnIt.value().data();
            if (!cell->rawValue().isEmpty())
                writer.writeTextElement(QStringLiteral("v"), cell->rawValue());
            writer.writeEndElement();//c
        }
        writer.writeEndElement();//row
    }
    writer.writeEndElement(); //sheetData

    writer.writeEndElement(); //worksheet
    writer.writeEndDocument();
    return true;
}

void WorksheetXmlPart::loadXmlElement_sheetPr(Mce::XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("sheetPr"));

    ws->sheetPr.attrs_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("tabColor"))
                ws->sheetPr.tabColor_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
            else if (reader.name() == QLatin1String("outlinePr"))
                ws->sheetPr.outlinePr_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
            else if (reader.name() == QLatin1String("pageSetUpPr"))
                ws->sheetPr.pageSetUpPr_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
        } else {
            if (reader.name() == QLatin1String("sheetPr"))
                break;
        }
    }
}

void WorksheetXmlPart::loadXmlElement_sheetView(Mce::XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("sheetView"));

    SheetView sheetView;
    sheetView.attrs_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("pane")) {
                sheetView.pane_raw = Ooxml::XmlHelper::xmlAttributesToHash(reader.attributes());
            } else if (reader.name() == QLatin1String("selection")) {
                //[0~4]
            } else if (reader.name() == QLatin1String("pivotSelection")) {
                //[0~4]
            }
        } else {
            if (reader.name() == QLatin1String("sheetView"))
                break;
        }
    }
    ws->sheetViews.append(sheetView);
}

void WorksheetXmlPart::loadXmlElement_row(Mce::XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("row"));

    QXmlStreamAttributes row_attrs = reader.attributes();
    int row_number = -1;
    if (row_attrs.hasAttribute(QLatin1String("r"))) {
        row_number = row_attrs.value(QLatin1String("r")).toInt();
    } else {
        //How do deal with this?
        if (ws->rows_raw.isEmpty())
            row_number = 1;
        else
            row_number = ws->rows_raw.lastKey() + 1;
    }
    ws->rows_raw.insert(row_number, Ooxml::XmlHelper::xmlAttributesToHash(row_attrs));

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("c")) {
                loadXmlElement_c(reader);
            }
        } else {
            if (reader.name() == QLatin1String("row"))
                break;
        }
    }
}

void WorksheetXmlPart::loadXmlElement_c(Mce::XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("c"));
    QXmlStreamAttributes attrs = reader.attributes();

    CellReference cellRef;
    if (attrs.hasAttribute(QLatin1String("r"))) {
        cellRef = CellReference(attrs.value(QLatin1String("r")).toString());
    } else {
        //In case the element has no "r" attribute.
        int row = ws->cellTable.lastKey();
        int col = ws->cellTable[row].isEmpty() ? 0 : (ws->cellTable[row].lastKey() + 1);
        cellRef = CellReference(row, col);
    }

    //Create a cell, then fill the private data of the data.
    CellPrivate *cell = ws->getCellPrivate(ws->createCell(cellRef.row(), cellRef.column()));
    cell->attrs_raw = Ooxml::XmlHelper::xmlAttributesToHash(attrs);

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("f")) {
                loadXmlElement_f(reader);
            } else if (reader.name() == QLatin1String("v")) {
                cell->v = reader.readElementText();
            } else if (reader.name() == QLatin1String("is")) {
                loadXmlElement_is(reader);
            }
        } else {
            if (reader.name() == QLatin1String("c"))
                break;
        }
    }
}

void WorksheetXmlPart::loadXmlElement_f(Mce::XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("f"));

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {

        } else {
            if (reader.name() == QLatin1String("f"))
                break;
        }
    }
}

void WorksheetXmlPart::loadXmlElement_is(Mce::XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("is"));
    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {

        } else {
            if (reader.name() == QLatin1String("is"))
                break;
        }
    }
}

} // namespace Sml
} // namespace QtOfficeOpenXml
