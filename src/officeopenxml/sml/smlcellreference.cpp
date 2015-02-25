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
#include <QtOfficeOpenXml/smlcellreference.h>

#include <QtCore/qregularexpression.h>

namespace QtOfficeOpenXml {
namespace Sml {

namespace {

int intPow(int x, int p)
{
    if (p == 0) return 1;
    if (p == 1) return x;

    int tmp = intPow(x, p/2);
    if (p%2 == 0) return tmp * tmp;
    else return x * tmp * tmp;
}

QString col_to_name(int col_num)
{
    QString col_str;
    int remainder;
    while (col_num) {
        remainder = col_num % 26;
        if (remainder == 0)
            remainder = 26;
        col_str.prepend(QChar('A'+remainder-1));
        col_num = (col_num - 1) / 26;
    }
    return col_str;
}

int col_from_name(const QString &col_str)
{
    int col = 0;
    int expn = 0;
    for (int i=col_str.size()-1; i>-1; --i) {
        col += (col_str[i].unicode() - 'A' + 1) * intPow(26, expn);
        expn++;
    }

    return col;
}

QString getEscapedSheetName(const QString &name)
{
    if (name.contains(QRegularExpression(QStringLiteral("[\\+\\-%&=<>]#!"))))
        return QStringLiteral("\'%1\'").arg(name);
    return name;
}
} //namespace

/*!
  \class QtOfficeOpenXml::Sml::CellReference

  "A1", "$A1", "A$1", "$A$1"
  "1:1", "$1:1", "1:$1", "$1:$1"
  "A:A", "$A:A", "A:$A", "$A:$A"
*/

/*!
    Constructs an invalid Cell Reference
*/
CellReference::CellReference()
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
}

/*!
    Constructs a single Cell Reference
*/
CellReference::CellReference(int row, int col)
    : m_row1(row), m_col1(col), m_row2(row), m_col2(col)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{

}

/*!
    Constructs a Cell range Reference
*/
CellReference::CellReference(int firstRow, int firstCol, int lastRow, int lastCol, Type flag)
    : m_row1(firstRow), m_col1(firstCol), m_row2(lastRow), m_col2(lastCol)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(flag)
{

}

/*!
    \overload
    Constructs the Reference form the given \a cell string.
*/
CellReference::CellReference(const QString &cell)
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
    init(cell);
}

/*!
    \overload
    Constructs the Reference form the given \a cell string.
*/
CellReference::CellReference(const char *cell)
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
    init(QString::fromUtf8(cell));
}

void CellReference::init(const QString &cell_str)
{
    int idx = cell_str.lastIndexOf(QLatin1Char('!'));
    if (idx != -1) {
        m_sheetName = cell_str.left(idx);
        if (m_sheetName.startsWith(QLatin1Char('\'')))
            m_sheetName = m_sheetName.mid(1, m_sheetName.size()-2);
    }

    QRegularExpression pattern_A1(QStringLiteral("^(\\$?)([A-Z]{1,3})(\\$?)(\\d+)$"));
    QRegularExpressionMatch match = pattern_A1.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = SingleCellType;
        m_isCol1Abs = !match.captured(1).isEmpty();
        m_col1 = col_from_name(match.captured(2));
        m_isRow1Abs = !match.captured(3).isEmpty();
        m_row1 = match.captured(4).toInt();
        return;
    }

    QRegularExpression pattern_A1A1(QStringLiteral("^(\\$?)([A-Z]{1,3})(\\$?)(\\d+):(\\$?)([A-Z]{1,3})(\\$?)(\\d+)$"));
    match = pattern_A1A1.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = CellRangeType;
        m_isCol1Abs = !match.captured(1).isEmpty();
        m_col1 = col_from_name(match.captured(2));
        m_isRow1Abs = !match.captured(3).isEmpty();
        m_row1 = match.captured(4).toInt();
        m_isCol2Abs = !match.captured(5).isEmpty();
        m_col2 = col_from_name(match.captured(6));
        m_isRow2Abs = !match.captured(7).isEmpty();
        m_row2 = match.captured(8).toInt();
        return;
    }

    QRegularExpression pattern_AA(QStringLiteral("^(\\$?)([A-Z]{1,3}):(\\$?)([A-Z]{1,3})$"));
    match = pattern_AA.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = ColumnsType;
        m_isCol1Abs = !match.captured(1).isEmpty();
        m_col1 = col_from_name(match.captured(2));
        m_isCol2Abs = !match.captured(3).isEmpty();
        m_col2 = col_from_name(match.captured(4));
        return;
    }

    QRegularExpression pattern_11(QStringLiteral("^(\\$?)(\\d+):(\\$?)(\\d+)$"));
    match = pattern_11.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = RowsType;
        m_isRow1Abs = !match.captured(1).isEmpty();
        m_row1 = match.captured(2).toInt();
        m_isRow2Abs = !match.captured(3).isEmpty();
        m_row2 = match.captured(4).toInt();
        return;
    }

    //Invalid string pattern
}

/*!
    Destroys the Reference.
*/
CellReference::~CellReference()
{
}

QString CellReference::toString() const
{
    return toString(m_isRow1Abs, m_isCol1Abs, m_isRow2Abs, m_isCol2Abs);
}

QString CellReference::toString(bool isRowAbs, bool isColAbs) const
{
    return toString(isRowAbs, isColAbs, isRowAbs, isColAbs);
}

/*!
     Convert the Reference to string notation, such as "A1" or "$A$1".
     If current object is invalid, an empty string will be returned.
*/
QString CellReference::toString(bool isRow1Abs, bool isCol1Abs, bool isRow2Abs, bool isCol2Abs) const
{
    if (isNull())
        return QString();
    QStringList items;
    //sheetName
    if (!m_sheetName.isEmpty()) {
        items.append(getEscapedSheetName(m_sheetName));
        items.append(QStringLiteral("!"));
    }
    //col1
    if (m_type != RowsType) {
        if (isCol1Abs)
            items.append(QStringLiteral("$"));
        items.append(col_to_name(m_col1));
    }
    //row1
    if (m_type != ColumnsType) {
        if (isRow1Abs)
            items.append(QStringLiteral("$"));
        items.append(QString::number(m_row1));
    }
    //single cell or not?
    if (m_type == SingleCellType)
        return items.join(QString());
    items.append(QStringLiteral(":"));
    //col2
    if (m_type != RowsType) {
        if (isCol2Abs)
            items.append(QStringLiteral("$"));
        items.append(col_to_name(m_col2));
    }
    //row2
    if (m_type != ColumnsType) {
        if (isRow2Abs)
            items.append(QStringLiteral("$"));
        items.append(QString::number(m_row2));
    }

    return items.join(QString());
}

/*!
 * Returns true if the Reference is invalid.
 */
bool CellReference::isNull() const
{
    return m_row1 < 1 || m_col1 < 1;
}

CellReference CellReference::fromRows(int firstRow, int lastRow)
{
    return CellReference(firstRow, 1, lastRow, 16384, RowsType);
}

CellReference CellReference::fromColumns(int firstColumn, int lastColumn)
{
    return CellReference(1, firstColumn, 1048576, lastColumn, ColumnsType);
}

bool CellReference::operator ==(const CellReference &other) const
{
    if (m_type != other.m_type)
        return false;

    if (m_type == SingleCellType)
        return m_row1==other.m_row1 && m_col1==other.m_col1;
    if (m_type == RowsType)
        return m_row1==other.m_row1 && m_row2==other.m_row2;
    if (m_type == ColumnsType)
        return m_col1==other.m_col1 && m_col2==other.m_col2;

    return m_row1==other.m_row1 && m_row2==other.m_row2 && m_col1==other.m_col1 && m_col2==other.m_col2;
}

bool CellReference::operator !=(const CellReference &other) const
{
    return !(*this == other);
}

} // namespace Sml
} // namespace QtOfficeOpenXml
