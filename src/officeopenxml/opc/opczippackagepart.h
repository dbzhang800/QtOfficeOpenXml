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
