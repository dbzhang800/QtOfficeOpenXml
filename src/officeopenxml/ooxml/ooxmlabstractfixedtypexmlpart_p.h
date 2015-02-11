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
#ifndef QTOFFICEOPENXML_OOXMLABSTRACTFIXEDTYPEXMLPART_H
#define QTOFFICEOPENXML_OOXMLABSTRACTFIXEDTYPEXMLPART_H

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

#include <QtOfficeOpenXml/officeopenxmlglobal.h>
#include <QtOfficeOpenXml/ooxmlschames.h>
class QIODevice;

namespace QtOfficeOpenXml {
namespace Opc {
class PackagePart;
}
namespace Ooxml {
class AbstractFixedTypeXmlPart
{
public:
    AbstractFixedTypeXmlPart();
    virtual ~AbstractFixedTypeXmlPart();

    QString partName() const;
    QString contentType() const;

    bool loadFromPackagePart(Opc::PackagePart *part);
    bool saveToPackagePart(Opc::PackagePart *part, SchameType schameType) const;

protected:
    virtual bool doLoadFromXml(QIODevice *part) = 0;
    virtual bool doSaveToXml(QIODevice *part, SchameType schameType) const = 0;

    QString m_partName;
    QString m_contentType;
};

} // namespace Ooxml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OOXMLABSTRACTFIXEDTYPEXMLPART_H
