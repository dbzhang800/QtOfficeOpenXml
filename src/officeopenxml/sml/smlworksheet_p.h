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
