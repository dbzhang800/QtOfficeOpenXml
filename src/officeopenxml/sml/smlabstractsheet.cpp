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
