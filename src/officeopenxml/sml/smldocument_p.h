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
