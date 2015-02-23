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
#ifndef QTOFFICEOPENXML_SML_SMLCELL_H
#define QTOFFICEOPENXML_SML_SMLCELL_H

#include <QtOfficeOpenXml/smlglobal.h>

class QVariant;

namespace QtOfficeOpenXml {
namespace Sml {

class WorksheetPrivate;
class CellPrivate;
class Q_OFFICEOPENXML_EXPORT Cell
{
public:
    enum CellType {
        BooleanType,      //t="b"
        NumberType,       //t="n" (default)
        ErrorType,        //t="e"
        SharedStringType, //t="s"
        StringType,       //t="str"
        InlineStringType  //t="inlineStr"
    };

    ~Cell();

    CellType cellType() const;
    QVariant value() const;
    QString rawValue() const;
    //Format format() const;

    //bool hasFormula() const;
    //CellFormula formula() const;

    //bool isDateTime() const;
    //QDateTime dateTime() const;

    //bool isRichString() const;

private:
    Q_DECLARE_PRIVATE(Cell)
    friend class WorksheetPrivate;
    Cell();
    CellPrivate *d_ptr;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLCELL_H
