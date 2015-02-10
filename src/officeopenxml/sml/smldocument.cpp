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
#include "smldocument.h"
#include "smldocument_p.h"

namespace QtOfficeOpenXml {
namespace Sml {

DocumentPrivate::DocumentPrivate(Document *q)
    :q_ptr(q)
{
}


/*! \class QtOfficeOpenXml::Sml::Document
 *
 *  \brief For .xlsx loading and saving.
 */

Document::Document(QObject *parent) :
    QObject(parent), d_ptr(new DocumentPrivate(this))
{
}

Document::Document(const QString &fileName, QObject *parent) :
    QObject(parent), d_ptr(new DocumentPrivate(this))
{
}

Document::Document(QIODevice *device, QObject *parent) :
    QObject(parent), d_ptr(new DocumentPrivate(this))
{
}

Document::~Document()
{
    delete d_ptr;
}

bool Document::save()
{
    return false;
}

bool Document::saveAs(const QString &fileName)
{
    return false;
}

bool Document::saveAs(QIODevice *device)
{
    return false;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
