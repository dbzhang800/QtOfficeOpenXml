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
