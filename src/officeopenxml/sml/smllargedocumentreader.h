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
#ifndef QTOFFICEOPENXML_SML_SMLLARGEDOCUMENTREADER_H
#define QTOFFICEOPENXML_SML_SMLLARGEDOCUMENTREADER_H

#include <QtOfficeOpenXml/officeopenxmlglobal.h>
#include <QtCore/qobject.h>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Sml {
class LargeDocumentReaderPrivate;

class Q_OFFICEOPENXML_EXPORT LargeDocumentReader : public QObject
{
    Q_OBJECT
public:
    explicit LargeDocumentReader(const QString &fileName, QObject *parent = 0);
    explicit LargeDocumentReader(QIODevice *device, QObject *parent = 0);
    ~LargeDocumentReader();

private:
    Q_DECLARE_PRIVATE(LargeDocumentReader)
    LargeDocumentReaderPrivate *d_ptr;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLLARGEDOCUMENTREADER_H
