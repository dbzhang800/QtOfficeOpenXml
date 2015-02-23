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
#ifndef QTOFFICEOPENXML_SML_ABSTRACTSHEET_H
#define QTOFFICEOPENXML_SML_ABSTRACTSHEET_H

#include <QtOfficeOpenXml/smlglobal.h>

namespace QtOfficeOpenXml {
namespace Sml {

class AbstractSheetPrivate;
class Q_OFFICEOPENXML_EXPORT AbstractSheet
{
    Q_DECLARE_PRIVATE(AbstractSheet)
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
    AbstractSheet(AbstractSheetPrivate *d);
    AbstractSheetPrivate *d_ptr;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_ABSTRACTSHEET_H
