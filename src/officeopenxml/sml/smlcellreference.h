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
#ifndef SMLCELLREFERENCE_H
#define SMLCELLREFERENCE_H

#include <QtOfficeOpenXml/smlglobal.h>

namespace QtOfficeOpenXml {
namespace Sml {
class  Q_OFFICEOPENXML_EXPORT CellReference
{
public:
    enum Type {
        SingleCellType,
        CellRangeType,
        RowsType,
        ColumnsType,
    };

    CellReference();
    CellReference(int row, int col);
    CellReference(int firstRow, int firstCol, int lastRow, int lastCol, Type type=CellRangeType);
    CellReference(const QString &cell_A1);
    CellReference(const char *cell_A1);
    ~CellReference();

    QString toString() const;
    QString toString(bool isRowAbs, bool isColAbs) const;
    QString toString(bool isRow1Abs, bool isCol1Abs, bool isRow2Abs, bool isCol2Abs) const;
    static CellReference fromRows(int firstRow, int lastRow);
    static CellReference fromColumns(int firstColumn, int lastColumn);
    bool isNull() const;

    inline int row() const { return m_row1; }
    inline int column() const { return m_col1; }
    inline int firstRow() const { return m_row1; }
    inline int firstColumn() const { return m_col1; }
    inline int lastRow() const { return m_row2; }
    inline int lastColumn() const { return m_col2; }
    inline int rowCount() const { return m_row2 - m_row1 + 1; }
    inline int columnCount() const { return m_col2 - m_col1 + 1; }
    inline Type type() const {return m_type; }
    inline QString sheetName() const {return m_sheetName; }

    bool operator ==(const CellReference &other) const;
    bool operator !=(const CellReference &other) const;

private:
    void init(const QString &cell);
    int m_row1;
    int m_col1;
    int m_row2;
    int m_col2;
    bool m_isRow1Abs;
    bool m_isCol1Abs;
    bool m_isRow2Abs;
    bool m_isCol2Abs;
    QString m_sheetName;
    Type m_type;
};

} // namespace Sml
} // namespace QtOfficeOpenXml
#endif // SMLCELLREFERENCE_H
