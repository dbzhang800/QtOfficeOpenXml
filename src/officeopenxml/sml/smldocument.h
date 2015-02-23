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
#ifndef QTOFFICEOPENXML_SML_SMLDOCUMENT_H
#define QTOFFICEOPENXML_SML_SMLDOCUMENT_H

#include <QtOfficeOpenXml/ooxmlabstractdocument.h>
#include <QtOfficeOpenXml/smlglobal.h>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Sml {
class AbstractSheet;
class Worksheet;
class Cell;
class CellReference;
class DocumentPrivate;
class Q_OFFICEOPENXML_EXPORT Document : public Ooxml::AbstractDocument
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = 0);
    explicit Document(const QString &fileName, QObject *parent = 0);
    explicit Document(QIODevice *device, QObject *parent = 0);
    ~Document();

    Cell *cellAt(const CellReference &cell) const;

    bool defineName(const QString &name, const QString &formula, const QString &comment=QString(), const QString &scope=QString());

    int sheetCount() const;
    AbstractSheet *addSheet(const QString &sheetName = QString(), SheetType type = ST_Worksheet);
    AbstractSheet *insertSheet(int index, const QString &sheetName = QString(), SheetType type = ST_Worksheet);

    bool setCurrentSheet(int index);
    AbstractSheet *sheet(int index) const;
    AbstractSheet *currentSheet() const;
    Worksheet *currentWorksheet() const;

private:
    Q_DECLARE_PRIVATE(Document)
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLDOCUMENT_H
