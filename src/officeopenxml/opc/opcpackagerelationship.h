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
#ifndef QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H
#define QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H

#include <QtOfficeOpenXml/officeopenxmlglobal.h>
#include <QtCore/qstring.h>

namespace QtOfficeOpenXml {
namespace Opc {

enum TargetMode
{
    External,
    Internal
};

class PackageRelationshipPrivate;
class Q_OFFICEOPENXML_EXPORT PackageRelationship
{
    Q_DECLARE_PRIVATE(PackageRelationship)
public:
    PackageRelationship(const QString &id, const QString &type, const QString &source, const QString &target, TargetMode targetMode=Internal);
    ~PackageRelationship();

    QString id() const;
    QString relationshipType() const;
    QString source() const;
    QString target() const;
    TargetMode targetMode() const;

private:
    PackageRelationshipPrivate *d_ptr;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H
