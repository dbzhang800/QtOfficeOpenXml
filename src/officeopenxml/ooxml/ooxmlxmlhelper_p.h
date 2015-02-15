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
#ifndef QTOFFICEOPENXML_OOXML_OOXMLXMLHELPER_P_H
#define QTOFFICEOPENXML_OOXML_OOXMLXMLHELPER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Office Open Xml API.  It exists for
// the convenience of the Qt Office Open Xml.  This header file may
// change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qhash.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qxmlstream.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

class XmlHelper
{
public:
    XmlHelper();

    static QHash<QString, QString> xmlAttributesToHash(const QXmlStreamAttributes &attrs);
    static QXmlStreamAttributes xmlAttributesFromHash(const QHash<QString, QString> &hash, const QStringList &orderedNames=QStringList());
};

} // namespace Ooxml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OOXML_OOXMLXMLHELPER_P_H
