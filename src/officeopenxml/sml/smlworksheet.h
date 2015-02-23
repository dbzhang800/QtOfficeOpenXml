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
#ifndef QTOFFICEOPENXML_SML_SMLWORKSHEET_H
#define QTOFFICEOPENXML_SML_SMLWORKSHEET_H

#include <QtOfficeOpenXml/smlabstractsheet.h>

namespace QtOfficeOpenXml {
namespace Sml {
class Cell;
class CellReference;
class DocumentPrivate;
class WorksheetPrivate;
class Q_OFFICEOPENXML_EXPORT Worksheet : public AbstractSheet
{
    Q_DECLARE_PRIVATE(Worksheet)
public:
    Worksheet(const QString &name, int id, SheetState state);
    ~Worksheet();

    Cell *writeNumber(const CellReference &cellRef, double value);

    Cell *cellAt(const CellReference &cellRef) const;

    SheetType sheetType() const Q_DECL_OVERRIDE;

private:
    friend class DocumentPrivate;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLWORKSHEET_H
