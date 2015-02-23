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

#include <private/smlcell_p.h>

namespace QtOfficeOpenXml {
namespace Sml {

static const char * cellTypeNameTable[] = {
    //Note, keep the save order as CellTypeEnum.
    "b",
    "n",
    "e",
    "s",
    "str",
    "inlineStr"
};

CellPrivate::CellPrivate(Cell *q) :
    q_ptr(q)
{
}


/*!
 * \class QtOffcieOpenXml::Sml::Cell
 *
 */

Cell::~Cell()
{
    delete d_ptr;
}

Cell::CellType Cell::cellType() const
{
    Q_D(const Cell);
    if (d->attrs_raw.contains(QStringLiteral("t"))) {
        QString t = d->attrs_raw[QStringLiteral("t")];
        for (int i=0; i<sizeof(cellTypeNameTable)/sizeof(cellTypeNameTable[0]); ++i) {
            if (QLatin1String(cellTypeNameTable[i]) == t)
                return (CellType)i;
        }
    }
    return NumberType;
}

/*!
 * \Return the value in Qt's value type.
 */
QVariant Cell::value() const
{
    return QVariant();
}

QString Cell::rawValue() const
{
    Q_D(const Cell);
    return d->v;
}

Cell::Cell() :
    d_ptr(new CellPrivate(this))
{
}

} // namespace Sml
} // namespace QtOfficeOpenXml
