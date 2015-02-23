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
#ifndef QTOFFICEOPENXML_OPC_OPCPACKAGEPART_H
#define QTOFFICEOPENXML_OPC_OPCPACKAGEPART_H

#include <QtOfficeOpenXml/opcpackagerelationship.h>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Opc {
class Package;
class PackagePartPrivate;
class Q_OFFICEOPENXML_EXPORT PackagePart
{
    Q_DECLARE_PRIVATE(PackagePart)
public:
    virtual ~PackagePart();
    QString partName() const;
    QString contentType() const;

    QIODevice *getDevice();
    void releaseDevice();

    PackageRelationship *relationship(const QString &id) const;
    QList<PackageRelationship *> relationships() const;
    QList<PackageRelationship *> getRelationshipsByType(const QString &type) const;
    PackageRelationship *createRelationship(const QString &target, TargetMode mode, const QString &type, const QString &id=QString());
    void deleteRelationship(const QString &id);

protected:
    friend class Package;

    PackagePart(PackagePartPrivate *d);
    virtual QIODevice *doGetDevice() = 0;
    virtual void doReleaseDevice() = 0;

    PackagePartPrivate *d_ptr;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCPACKAGEPART_H
