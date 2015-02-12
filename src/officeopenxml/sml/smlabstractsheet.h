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
#ifndef QTOFFICEOPENXML_SML_ABSTRACTSHEET_H
#define QTOFFICEOPENXML_SML_ABSTRACTSHEET_H

#include <QtOfficeOpenXml/smlglobal.h>

namespace QtOfficeOpenXml {
namespace Sml {

class AbstractSheetPrivate;
class Q_OFFICEOPENXML_EXPORT AbstractSheet
{
public:
    virtual ~AbstractSheet();

    QString sheetName() const;
    virtual SheetType sheetType() const = 0;
    SheetState sheetState() const;
    void setSheetState(SheetState ss);
    bool isHidden() const;
    bool isVisible() const;
    void setHidden(bool hidden);
    void setVisible(bool visible);

    int sheetId() const;

protected:
    Q_DECLARE_PRIVATE(AbstractSheet)
    AbstractSheet(AbstractSheetPrivate *d);
    AbstractSheetPrivate *d_ptr;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_ABSTRACTSHEET_H
