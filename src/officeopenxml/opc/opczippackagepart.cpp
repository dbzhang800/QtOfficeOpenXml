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

#include "opczippackagepart.h"
#include "opczippackagepart_p.h"
#include "opczippackage.h"

#include <kzip.h>
#include <kzipfileentry.h>

#include <QDebug>

namespace QtOfficeOpenXml {
namespace Opc {

/*!
 * \class QtOfficeOpenXml::Opc::ZipPackagePartPrivate
 * \internal
 */
ZipPackagePartPrivate::ZipPackagePartPrivate(const QString &partName, const QString &type, PackagePart *q, Package *package)
    :PackagePartPrivate(partName, type, q, package), zipArchive(0), zipFileEntry(0), device(0)
{
}

/*
   \class ZipPartWriteDevice
*/
ZipPartWriteDevice::ZipPartWriteDevice(KZip *zip, const QString &partName)
    :archive(zip), partName(partName), dataLength(0)
{
    //Note, The size doesn't used by Kzip, so we can assign any value such as 1024.
    //Note, Remove the start '/' from the partName
    if (archive->prepareWriting(partName.mid(1), QStringLiteral("user"), QStringLiteral("group"), 1024))
        open(WriteOnly);
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
 * \class QtOfficeOpenXml::Opc::ZipPackagePart
 * \inmodule QtOfficeOpenXml
 */

ZipPackagePart::ZipPackagePart(const QString &partName, const QString &contentType, Package *package)
    :PackagePart(new ZipPackagePartPrivate(partName, contentType, this, package))
{

}

ZipPackagePart::~ZipPackagePart()
{
    doReleaseDevice();
}

QIODevice *ZipPackagePart::doGetDevice(QIODevice::OpenMode mode)
{
    Q_D(ZipPackagePart);
    if (d->device) {
        if (d->device->openMode() != mode) {
            qWarning("Users must release last open part device.");
            return 0;
        }
        return d->device;
    }

    if (mode == QIODevice::ReadOnly) {
        d->device = d->zipFileEntry->createDevice();
    } else if (mode == QIODevice::WriteOnly) {
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
