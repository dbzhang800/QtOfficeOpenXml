/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang <hello@debao.me>
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
#ifndef MCEXMLSTREAMREADER_P_H
#define MCEXMLSTREAMREADER_P_H

#include "mcexmlstreamreader.h"
#include <QtCore/qstringlist.h>
#include <QtCore/qstack.h>

namespace QtOfficeOpenXml {
namespace Mce {

class XmlStreamReaderPrivate
{
    Q_DECLARE_PUBLIC(XmlStreamReader)
public:
    XmlStreamReaderPrivate(QXmlStreamReader *reader, XmlStreamReader *q);
    ~XmlStreamReaderPrivate();

    QSet<QString> mceCurrentNamespaces;
    QHash<QString, QString> mceObsoleteNamespaces;
    QHash<QString, int> nonUnderstoodNamespacesCache;
    QStack<QSet<QString> > nonUnderstoodNamespacesStack;

    QXmlStreamReader *reader;
    XmlStreamReader *q_ptr;
};

} // namespace Mce
} // namespace QtOfficeOpenXml

#endif // MCEXMLSTREAMREADER_P_H
