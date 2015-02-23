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
#include <private/smlworkbook_p.h>

namespace QtOfficeOpenXml {
namespace Sml {

Workbook::Workbook()
{

}

QString Workbook::bookView(const QString &attribute) const
{
    if (bookViews_raw.isEmpty())
        return QString();
    if (!bookViews_raw[0].contains(attribute))
        return QString();
    return bookViews_raw[0][attribute].toInt();
}

void Workbook::setBookView(const QString &attribute, const QString &val)
{
    if (bookViews_raw.isEmpty())
        return bookViews_raw.append(QHash<QString, QString>());

    bookViews_raw[0][attribute] = val;
}

int Workbook::activeTab() const
{
    return bookView(QStringLiteral("activeTab")).toInt();
}

void Workbook::setActiveTab(int index)
{
    setBookView(QStringLiteral("activeTab"), QString::number(index));
}

} // namespace Sml
} // namespace QtOfficeOpenXml
