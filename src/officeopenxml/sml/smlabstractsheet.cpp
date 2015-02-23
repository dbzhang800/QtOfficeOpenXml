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
#include <private/smlabstractsheet_p.h>

namespace QtOfficeOpenXml {
namespace Sml {

AbstractSheetPrivate::AbstractSheetPrivate(const QString &name, int id, SheetState state, AbstractSheet *q)
    :name(name), id(id), sheetState(state), q_ptr(q)
{

}

AbstractSheetPrivate::~AbstractSheetPrivate()
{

}

/*! \class QtOfficeOpenXml::Sml::AbstractSheet
 *
 * \brief Base class for worksheet and chartsheet.
 */

AbstractSheet::~AbstractSheet()
{
    delete d_ptr;
}

QString AbstractSheet::sheetName() const
{
    Q_D(const AbstractSheet);
    return d->name;
}

SheetState AbstractSheet::sheetState() const
{
    Q_D(const AbstractSheet);
    return d->sheetState;
}

void AbstractSheet::setSheetState(SheetState ss)
{
    Q_D(AbstractSheet);
    d->sheetState = ss;
}

/*!
 * Returns true if the sheet is not visible, otherwise false will be returned.
 *
 * \sa sheetState(), setHidden()
 */
bool AbstractSheet::isHidden() const
{
    Q_D(const AbstractSheet);
    return d->sheetState != SS_Visible;
}

/*!
 * Returns true if the sheet is visible.
 */
bool AbstractSheet::isVisible() const
{
    return !isHidden();
}

/*!
 * Make the sheet hiden or visible based on \a hidden.
 */
void AbstractSheet::setHidden(bool hidden)
{
    Q_D(AbstractSheet);
    if (hidden == isHidden())
        return;

    d->sheetState = hidden ? SS_Hidden : SS_Visible;
}

/*!
 * Convenience function, equivalent to setHidden(! \a visible).
 */
void AbstractSheet::setVisible(bool visible)
{
    setHidden(!visible);
}

/*!
 * \internal
 */
int AbstractSheet::sheetId() const
{
    Q_D(const AbstractSheet);
    return d->id;
}

AbstractSheet::AbstractSheet(AbstractSheetPrivate *d)
    :d_ptr(d)
{
}


} // namespace Sml
} // namespace QtOfficeOpenXml
