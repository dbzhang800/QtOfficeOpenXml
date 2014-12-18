/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang <hello@debao.me>
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
#ifndef OPCZIPPACKAGEPART_P_H
#define OPCZIPPACKAGEPART_P_H

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

#include "opcpackagepart_p.h"
#include "opczippackagepart.h"

#include <QIODevice>

class KZip;
class KZipFileEntry;

namespace QtOfficeOpenXml {
namespace Opc {

class ZipPackagePartPrivate : public PackagePartPrivate
{
public:
    ZipPackagePartPrivate(const QString &partName, const QString type, PackagePart *q, Package *package);

    KZip *zipArchive;
    const KZipFileEntry *zipFileEntry;
    QIODevice *device;
};

/*!
 * \class ZipPartWriteDevice
 *
 * write data to ZipPackagePart
 */
class ZipPartWriteDevice : public QIODevice
{
public:
    ZipPartWriteDevice(KZip *zip, const QString &partName);
    ~ZipPartWriteDevice();

    void close() Q_DECL_OVERRIDE;
private:
    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 len) Q_DECL_OVERRIDE;

    KZip *archive;
    QString partName;
    qint64 dataLength;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // OPCZIPPACKAGEPART_P_H
