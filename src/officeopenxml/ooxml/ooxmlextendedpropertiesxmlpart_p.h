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
#ifndef QTOFFICEOPENXML_OOXML_OOXMLDOCPROPSAPPPART_H
#define QTOFFICEOPENXML_OOXML_OOXMLDOCPROPSAPPPART_H

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

#include <private/ooxmlabstractfixedtypexmlpart_p.h>

namespace QtOfficeOpenXml {
namespace Ooxml {
class AbstractDocumentPrivate;
class ExtendedPropertiesXmlPart : public AbstractFixedTypeXmlPart
{
public:
    ExtendedPropertiesXmlPart(AbstractDocumentPrivate *documentData);

    QString contentType() const Q_DECL_OVERRIDE;

private:
    bool doLoadFromXml(QIODevice *part) Q_DECL_OVERRIDE;
    bool doSaveToXml(QIODevice *part, SchameType schameType) const Q_DECL_OVERRIDE;

    QString getApplicationName() const;
    QString getApplicationVersion() const;
    QString getCompany() const;
    AbstractDocumentPrivate *m_documentData;
};

} // namespace Ooxml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OOXML_OOXMLDOCPROPSAPPPART_H
