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

#include "opczippackagepart.h"
#include "opczippackagepart_p.h"
#include "opczippackage.h"

#include <kzip.h>
#include <kzipfileentry.h>

#include <QDebug>

namespace QtOfficeOpenXml {
namespace Opc {

ZipPackagePartPrivate::ZipPackagePartPrivate(const QString &partName, const QString type, PackagePart *q, Package *package)
    :PackagePartPrivate(partName, type, q, package), zipFileEntry(0), device(0)
{
}

/*
   \class ZipPartWriteDevice
*/
ZipPartWriteDevice::ZipPartWriteDevice(KZip *zip, const QString &partName)
    :archive(zip), partName(partName), dataLength(0)
{
    open(WriteOnly);
    //Note, The size doesn't used by Kzip, so we can assign any value such as 1024.
    //Note, Remove the start '/' from the partName
    archive->prepareWriting(partName.mid(1), QStringLiteral("user"), QStringLiteral("group"), 1024);
}

ZipPartWriteDevice::~ZipPartWriteDevice()
{
    close();
}

qint64 ZipPartWriteDevice::readData(char * /*data*/, qint64 /*maxlen*/)
{
    return -1;
}

qint64 ZipPartWriteDevice::writeData(const char *data, qint64 len)
{
    //Note, the return value of KZip::writeData() is boolean.
    if (archive->writeData(data, len)) {
        dataLength += len;
        return len;
    }
    return -1;
}

void ZipPartWriteDevice::close()
{
    if (isOpen()) {
        QIODevice::close();
        archive->finishWriting(dataLength);
    }
}

/*!
 * \class ZipPackagePart
 */

ZipPackagePart::ZipPackagePart(const QString &partName, const QString &contentType, Package *package)
    :PackagePart(new ZipPackagePartPrivate(partName, contentType, this, package))
{

}

ZipPackagePart::~ZipPackagePart()
{
    doReleaseDevice();
}

QIODevice *ZipPackagePart::doGetDevice()
{
    Q_D(ZipPackagePart);
    if (d->device)
        return d->device;

    if (d->package->mode() == QIODevice::ReadOnly) {
        d->device = d->zipFileEntry->createDevice();
    } else if (d->package->mode() == QIODevice::WriteOnly) {
        d->device = new ZipPartWriteDevice(d->zipArchive, d->partName);
    }
    return d->device;
}

void ZipPackagePart::doReleaseDevice()
{
    Q_D(ZipPackagePart);
    if (!d->device)
        return;

    d->device->close();
    delete d->device;
    d->device = 0;
}

} // namespace Opc
} // namespace QtOfficeOpenXml
