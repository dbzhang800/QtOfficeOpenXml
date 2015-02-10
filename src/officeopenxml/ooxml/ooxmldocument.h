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
#ifndef QTOFFICEOPENXML_OOXML_DOCUMENT_H
#define QTOFFICEOPENXML_OOXML_DOCUMENT_H

#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtCore/qobject.h>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Ooxml {

class DocumentPrivate;
class Q_OFFICEOPENXML_EXPORT Document : public QObject
{
    Q_OBJECT
public:
    ~Document();

    bool save(SchameType schameType=UnknownSchame) const;
    bool saveAs(const QString &fileName, SchameType schameType=UnknownSchame) const;
    bool saveAs(QIODevice *device, SchameType schameType=UnknownSchame) const;

protected:
    Q_DECLARE_PRIVATE(Document)
    Document(DocumentPrivate *d, QObject *parent);
    DocumentPrivate *d_ptr;
};

} // namespace Ooxml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OOXML_DOCUMENT_H
