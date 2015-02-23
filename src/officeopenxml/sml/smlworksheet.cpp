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
#include <private/smlworksheet_p.h>
#include <private/smlcell_p.h>
#include <QtOfficeOpenXml/smlcellreference.h>

namespace QtOfficeOpenXml {
namespace Sml {

WorksheetPrivate::WorksheetPrivate(const QString &name, int id, SheetState state, Worksheet *q) :
    AbstractSheetPrivate(name, id, state, q)
{
}

Cell *WorksheetPrivate::createCell(int row, int column)
{
    QSharedPointer<Cell> cell(new Cell());
    cellTable[row][column] = cell;

    return cell.data();
}

Cell *WorksheetPrivate::getCell(int row, int column) const
{
    if (!cellTable.contains(row))
        return 0;
    if (!cellTable[row].contains(column))
        return 0;
    return cellTable[row][column].data();
}

/*!
 * \internal
 * Helper function.
 */
CellPrivate *WorksheetPrivate::getCellPrivate(Cell *cell)
{
    if (!cell)
        return 0;
    return cell->d_ptr;
}

Worksheet::Worksheet(const QString &name, int id, SheetState state) :
    AbstractSheet(new WorksheetPrivate(name, id, state, this))
{
}

Worksheet::~Worksheet()
{
}

Cell *Worksheet::writeNumber(const CellReference &cellRef, double value)
{
    Q_D(Worksheet);
    if (cellRef.isNull())
        return 0;
    if (!cellRef.sheetName().isEmpty() && cellRef.sheetName() != sheetName())
        return 0;

    Cell *cell = d->createCell(cellRef.row(), cellRef.column());
    d->getCellPrivate(cell)->v = QString::number(value);

    return cell;
}

Cell *Worksheet::cellAt(const CellReference &cellRef) const
{
    Q_D(const Worksheet);
    return d->getCell(cellRef.row(), cellRef.column());
}

SheetType Worksheet::sheetType() const
{
    return ST_Worksheet;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
