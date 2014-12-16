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
#ifndef OOXMLZIPPACKAGE_H
#define OOXMLZIPPACKAGE_H

#include "opcpackage.h"

namespace QtOfficeOpenXml {
namespace Opc {
class ZipPackagePart;
class ZipPackagePrivate;
class Q_OFFICEOPENXML_EXPORT ZipPackage : public Package
{
public:
    explicit ZipPackage(const QString &fileName);
    explicit ZipPackage(QIODevice *device);
    virtual ~ZipPackage();

private:
    bool doOpenPackage(QIODevice::OpenMode mode) Q_DECL_OVERRIDE;
    bool doClosePackage() Q_DECL_OVERRIDE;
    PackagePart *doCreatePart(const QString &partName, const QString &contentType) Q_DECL_OVERRIDE;
    bool doDeletePart(const QString &partName) Q_DECL_OVERRIDE;

    Q_DECLARE_PRIVATE(ZipPackage)
};

} // namespace Opc
} // namespace QtOfficeOpenXml
#endif // OOXMLZIPPACKAGE_H
