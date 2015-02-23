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
#ifndef QTOFFICEOPENXML_OOXML_DOCUMENT_H
#define QTOFFICEOPENXML_OOXML_DOCUMENT_H

#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtCore/qobject.h>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Ooxml {

class AbstractDocumentPrivate;
class Q_OFFICEOPENXML_EXPORT AbstractDocument : public QObject
{
    Q_OBJECT
public:
    enum DocumentProperty {
        //Core Property of the Opc Package
        DP_Creator,
        DP_Identifier,
        DP_Title,
        DP_Subject,
        DP_Description,
        DP_Language,
        DP_Created,
        DP_Modified,
        DP_ContentType,
        DP_Keywords,
        DP_Category,
        DP_Version,
        DP_LastModifiedBy,
        DP_ContentStatus,
        DP_Revision,
        DP_LastPrinted,
        //App Property of the Office Document
        DP_Manager = 0x0100,
        DP_Company,
        DP_Application,
        DP_AppVersion
    };

    ~AbstractDocument();

    QVariant documentProperty(DocumentProperty name) const;
    void setDocumentProperty(DocumentProperty name, const QVariant &property);

    bool save(SchameType schameType=UnknownSchame) const;
    bool saveAs(const QString &fileName, SchameType schameType=UnknownSchame) const;
    bool saveAs(QIODevice *device, SchameType schameType=UnknownSchame) const;

protected:
    AbstractDocument(AbstractDocumentPrivate *d, QObject *parent);
    AbstractDocumentPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractDocument)
};

} // namespace Ooxml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OOXML_DOCUMENT_H
