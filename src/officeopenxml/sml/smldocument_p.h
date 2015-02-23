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
#ifndef QTOFFICEOPENXML_SML_SMLDOCUMENT_P_H
#define QTOFFICEOPENXML_SML_SMLDOCUMENT_P_H

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

#include <private/ooxmlabstractdocument_p.h>
#include <private/smlworkbook_p.h>
#include <QtOfficeOpenXml/smldocument.h>

namespace QtOfficeOpenXml {
namespace Opc {
class Package;
}
namespace Sml {
class DocumentPrivate : public Ooxml::AbstractDocumentPrivate
{
    Q_DECLARE_PUBLIC(Document)
public:
    explicit DocumentPrivate(Document *q);
    bool doLoadPackage(Opc::Package *package) Q_DECL_OVERRIDE;
    bool doSavePackage(Opc::Package *package, Ooxml::SchameType schame) const Q_DECL_OVERRIDE;
    QStringList sheetNames() const;
    Worksheet *getWorksheetByName(const QString &sheetName) const;
    QSharedPointer<AbstractSheet> createSheet(const QString &sheetName, SheetType type);

    Workbook wb;
    QList<QSharedPointer<AbstractSheet> > sheets;
    int lastSheetId;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLDOCUMENT_P_H
