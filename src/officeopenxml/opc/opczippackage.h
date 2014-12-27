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
#ifndef OOXMLZIPPACKAGE_H
#define OOXMLZIPPACKAGE_H

#include <QtOfficeOpenXml/opcpackage.h>

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
