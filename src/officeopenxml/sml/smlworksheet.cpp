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
