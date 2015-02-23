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

#include <private/opcpackagepart_p.h>
#include <QtOfficeOpenXml/opczippackagepart.h>
#include <QtCore/qiodevice.h>

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
