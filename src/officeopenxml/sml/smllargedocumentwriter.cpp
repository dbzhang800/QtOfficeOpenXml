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
#include "smllargedocumentwriter.h"
#include "smllargedocumentwriter_p.h"
#include "opcpackage.h"
#include "opcpackagepart.h"

namespace QtOfficeOpenXml {
namespace Sml {

LargeDocumentWriterPrivate::LargeDocumentWriterPrivate(LargeDocumentWriter *q) :
    isClosed(false), ooxmlSchameType(Ooxml::TransitionalSchame),
    q_ptr(q)
{
}

/*!
 * \class QtOfficeOpenXml::Sml::LargeDocumentWriter
 *
 * \brief This class is designed for generating large .xlsx file.
 */

LargeDocumentWriter::LargeDocumentWriter(const QString &fileName, QObject *parent) :
    QObject(parent), d_ptr(new LargeDocumentWriterPrivate(this))
{
    d_ptr->package = Opc::Package::open(fileName, QIODevice::WriteOnly);
    if (!d_ptr->package) {
        qWarning("Can't open the package");
        d_ptr->isClosed = true;
    }
}

LargeDocumentWriter::LargeDocumentWriter(QIODevice *device, QObject *parent) :
    QObject(parent), d_ptr(new LargeDocumentWriterPrivate(this))
{
    d_ptr->package = Opc::Package::open(device, QIODevice::WriteOnly);
    if (!d_ptr->package) {
        qWarning("Can't open the package");
        d_ptr->isClosed = true;
    }
}

LargeDocumentWriter::~LargeDocumentWriter()
{
    close();
    if (d_ptr->package)
        delete d_ptr->package;
    delete d_ptr;
}

/*!
 * Must be called before anything is written.
 *
 * The default type is QtOfficeOpen::Ooxml::TransitionalSchame.
 */
bool LargeDocumentWriter::setOoxmlSchameType(Ooxml::SchameType type)
{
    Q_D(LargeDocumentWriter);
    d->ooxmlSchameType = type;
    return true;
}

bool LargeDocumentWriter::close()
{
    Q_D(LargeDocumentWriter);
    if (d->isClosed)
        return true;
    //Todo.
    d->package->close();
    d->isClosed = true;
    return true;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
