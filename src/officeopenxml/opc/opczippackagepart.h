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
#ifndef QTOFFICEOPEN_OPC_OPCZIPPACKAGEPART_H
#define QTOFFICEOPEN_OPC_OPCZIPPACKAGEPART_H

#include <QtOfficeOpenXml/opcpackagepart.h>

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
