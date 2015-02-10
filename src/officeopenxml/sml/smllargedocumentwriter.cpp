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
#include "smllargedocumentwriter.h"
#include "smllargedocumentwriter_p.h"

namespace QtOfficeOpenXml {
namespace Sml {

LargeDocumentWriterPrivate::LargeDocumentWriterPrivate(LargeDocumentWriter *q) :
    isClosed(false), ooxmlSchameTye(Ooxml::TransitionalSchame),
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
}

LargeDocumentWriter::LargeDocumentWriter(QIODevice *device, QObject *parent) :
    QObject(parent), d_ptr(new LargeDocumentWriterPrivate(this))
{
}

LargeDocumentWriter::~LargeDocumentWriter()
{
    close();
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
    d->ooxmlSchameTye = type;
    return true;
}

bool LargeDocumentWriter::close()
{
    Q_D(LargeDocumentWriter);
    if (d->isClosed)
        return true;
    //Todo.
    d->isClosed = true;
    return true;
}

} // namespace Sml
} // namespace QtOfficeOpenXml
