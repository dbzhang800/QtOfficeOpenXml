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
#ifndef QTOFFICEOPENXML_SML_SMLWORKSHEET_P_H
#define QTOFFICEOPENXML_SML_SMLWORKSHEET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Office Open Xml API.  It exists for
// the convenience of the Qt Office Open Xml.  This header file may
// change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <private/smlabstractsheet_p.h>
#include <QtOfficeOpenXml/smlworksheet.h>

#include <QtCore/qhash.h>
#include <QtCore/qmap.h>
#include <QtCore/qsharedpointer.h>

namespace QtOfficeOpenXml {
namespace Sml {
class Cell;
class CellPrivate;
class WorksheetPrivate : public AbstractSheetPrivate
{
    Q_DECLARE_PUBLIC(Worksheet)
public:
    WorksheetPrivate(const QString &name, int id, SheetState state, Worksheet *q);

    Cell *createCell(int row, int column);
    Cell *getCell(int row, int column) const;
    static CellPrivate *getCellPrivate(Cell *cell);

    //baseColWidth, defaultColWidth, defaultRowHeight, customHeight, zeroHeight,
    //thickTop, thickBottom, outlineLevelRow, outlineLevelCol,
    QHash<QString, QString> sheetFormatPr_raw;

    //min, max, width, style, hidden, bestFit, customWidth,
    //phonetic, outlineLevel, collapsed,
    QMap<int, QHash<QString, QString> > cols_raw;

    //r, spans, s, customFormat, ht, hidden, customHeight, outlineLevel,
    //collapsed, thickTop, thickBot, ph,
    QMap<int, QHash<QString, QString> > rows_raw;

    QMap<int, QMap<int, QSharedPointer<Cell> > > cellTable;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLWORKSHEET_P_H
