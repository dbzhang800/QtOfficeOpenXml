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
