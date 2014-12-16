/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang
** Contact: hello@debao.me
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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
