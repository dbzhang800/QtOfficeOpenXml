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
    return bookViews_raw[0][attribute];
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
