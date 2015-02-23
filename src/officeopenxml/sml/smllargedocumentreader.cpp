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
#include "smllargedocumentreader.h"
#include "smllargedocumentreader_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"

#include <QtCore/qsharedpointer.h>

namespace QtOfficeOpenXml {
namespace Sml {

LargeDocumentReaderPrivate::LargeDocumentReaderPrivate(LargeDocumentReader *q) :
    ooxmlSchameType(Ooxml::TransitionalSchame),
    q_ptr(q)
{
}

/*!
 * \class QtOfficeOpenXml::Sml::LargeDocumentReader
 *
 * \brief This class is designed for loading large .xlsx file.
 */

LargeDocumentReader::LargeDocumentReader(const QString &fileName, QObject *parent) :
    QObject(parent), d_ptr(new LargeDocumentReaderPrivate(this))
{
    d_ptr->package = Opc::Package::open(fileName, QIODevice::ReadOnly);
    if (!d_ptr->package) {
        qWarning("Can't open the package");
    }
}


LargeDocumentReader::LargeDocumentReader(QIODevice *device, QObject *parent) :
    QObject(parent), d_ptr(new LargeDocumentReaderPrivate(this))
{
    d_ptr->package = Opc::Package::open(device, QIODevice::ReadOnly);
    if (!d_ptr->package) {
        qWarning("Can't open the package");
    }
}

LargeDocumentReader::~LargeDocumentReader()
{
    if (d_ptr->package)
        delete d_ptr->package;
    delete d_ptr;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
