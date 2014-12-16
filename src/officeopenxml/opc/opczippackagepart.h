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
#ifndef QTOFFICEOPEN_OPC_OPCZIPPACKAGEPART_H
#define QTOFFICEOPEN_OPC_OPCZIPPACKAGEPART_H

#include "opcpackagepart.h"

namespace QtOfficeOpenXml {
namespace Opc {

class ZipPackage;
class ZipPackagePrivate;
class ZipPackagePartPrivate;
class ZipPackagePart : public PackagePart
{
public:
    ~ZipPackagePart();

private:
    friend class ZipPackage;
    friend class ZipPackagePrivate;

    ZipPackagePart(const QString &partName, const QString &contentType, Package *package);
    QIODevice *doGetDevice() Q_DECL_OVERRIDE;
    void doReleaseDevice() Q_DECL_OVERRIDE;

    Q_DECLARE_PRIVATE(ZipPackagePart)
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPEN_OPC_OPCZIPPACKAGEPART_H
